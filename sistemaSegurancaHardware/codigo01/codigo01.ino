#include <ESP8266WiFi.h>          // Biblioteca do Wi-Fi
#include <Firebase_ESP_Client.h>  // Biblioteca do Firebase
#include <addons/TokenHelper.h>   // Informações sobre geração de token
#include <addons/RTDBHelper.h>    // Funções auxiliares para o RTDB

#define lampada D8
#define buzzer D7
#define infra_v D6
#define motor D5
#define vibracao D4

int defesa = 0;    // Estado de defesa
int state = LOW;   // Estado inicial (LOW = desligado, HIGH = ligado)
int seta = 0;      // Flag para alternância de estado

const char* ssid = "S23 Ultra de Vinicius Gabriel";       // Nome da rede
const char* password = "pcgkkdr9y54r5qn";         // Senha da rede

/* API Key do Firebase */
#define API_KEY "AIzaSyArgORFATJG4ou2ca-s8dvk95l6AZxzs-Y"

/* URL do Realtime Database */
#define DATABASE_URL "https://sistema-seguranca-9f070-default-rtdb.firebaseio.com/"  

/* Credenciais do usuário Firebase */
#define USER_EMAIL "projetoint7@gmail.com"
#define USER_PASSWORD "Senha@123456"

// Objeto do Firebase
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

void setup() {
  // Inicialização do Serial Monitor
  Serial.begin(115200);
  
  // Configuração do Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Conectando ao Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nConectado ao Wi-Fi!");

  // Configuração do Firebase
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  // Configuração de autenticação
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  // Inicializa o Firebase
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // Verifica se conectou ao Firebase
  if (!Firebase.RTDB.beginStream(&fbdo, "/defesa")) {
    Serial.println("Erro ao iniciar o stream do Firebase: " + fbdo.errorReason());
  } else {
    Serial.println("Conectado ao Firebase!");
  }

  // Configuração dos pinos
  pinMode(lampada, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(motor, OUTPUT);
  pinMode(infra_v, INPUT);
  pinMode(vibracao, INPUT);
}

void loop() {
  // Ler o estado de "defesa" do banco de dados
  if (Firebase.RTDB.getInt(&fbdo, "/defesa")) {
    defesa = fbdo.intData();  // Atualiza o estado de defesa com o valor lido do Firebase
    Serial.print("Estado de defesa: ");
    Serial.println(defesa);
  } else {
    Serial.println("Falha ao ler do Firebase: " + fbdo.errorReason());
  }

  // Modo de operação com defesa desativada
  if (defesa == 0) {
    if (digitalRead(infra_v) == HIGH && seta == 0) {
      seta = 1;  // Marca que o estado foi alterado
      state = !state;  // Alterna o estado (liga/desliga)
      digitalWrite(lampada, state);
      digitalWrite(motor, state);

      // Enviar estado da lâmpada para o banco de dados
      if (Firebase.RTDB.setInt(&fbdo, "/lampada", state)) {
        Serial.println("Estado da lâmpada enviado ao Firebase!");
      } else {
        Serial.println("Erro ao enviar estado da lâmpada: " + fbdo.errorReason());
      }

      if (Firebase.RTDB.setInt(&fbdo, "/motor", state)) {
        Serial.println("Estado da motor enviado ao Firebase!");
      } else {
        Serial.println("Erro ao enviar estado da motor: " + fbdo.errorReason());
      }
    }
    if (digitalRead(infra_v) == LOW) {
      seta = 0;  // Permite alternar novamente
    }
  }
  // Modo de operação com defesa ativada
  else {
    if (digitalRead(infra_v) == LOW || digitalRead(vibracao) == HIGH) {      
      // Enviar alerta ao banco de dados
      if (Firebase.RTDB.setInt(&fbdo, "/buzzer", 1)) {
        Serial.println("Alerta enviado ao Firebase!");
      } else {
        Serial.println("Erro ao enviar alerta: " + fbdo.errorReason());
      }
      for(int i = 0; i < 5; i++){
        digitalWrite(buzzer, HIGH);  // Liga o buzzer
        delay(500);  // Aguarda 0,5 segundos
        digitalWrite(buzzer, LOW);  // Desliga o buzzer
        delay(50);  // Aguarda 0,5 segundos

        digitalWrite(buzzer, HIGH);  // Liga o buzzer
        delay(250);  // Aguarda 0,5 segundos
        digitalWrite(buzzer, LOW);  // Desliga o buzzer
        delay(50);  // Aguarda 0,5 segundos

        digitalWrite(buzzer, HIGH);  // Liga o buzzer
        delay(250);  // Aguarda 0,5 segundos
        digitalWrite(buzzer, LOW);  // Desliga o buzzer
        delay(50);  // Aguarda 0,5 segundos

        digitalWrite(buzzer, HIGH);  // Liga o buzzer
        delay(500);  // Aguarda 0,5 segundos
        digitalWrite(buzzer, LOW);  // Desliga o buzzer
        delay(50);  // Aguarda 0,5 segundos

        digitalWrite(buzzer, HIGH);  // Liga o buzzer
        delay(500);  // Aguarda 0,5 segundos
        digitalWrite(buzzer, LOW);  // Desliga o buzzer
        delay(500);  // Aguarda 0,5 segundos

        digitalWrite(buzzer, HIGH);  // Liga o buzzer
        delay(500);  // Aguarda 0,5 segundos
        digitalWrite(buzzer, LOW);  // Desliga o buzzer
        delay(50);  // Aguarda 0,5 segundos

        digitalWrite(buzzer, HIGH);  // Liga o buzzer
        delay(500);  // Aguarda 0,5 segundos
        digitalWrite(buzzer, LOW);  // Desliga o buzzer
        delay(50);  // Aguarda 0,5 segundos
      }
      if (Firebase.RTDB.setInt(&fbdo, "/buzzer", 0)) {
        Serial.println("Alerta enviado ao Firebase!");
      } else {
        Serial.println("Erro ao enviar alerta: " + fbdo.errorReason());
      }
    }
    digitalWrite(buzzer,LOW);
  }

  delay(1000);  // Aguarda 1 segundo antes da próxima iteração
}