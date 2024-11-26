package br.com.sistemasegurancaapk

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.compose.foundation.layout.*
import androidx.compose.material3.Button
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import br.com.sistemasegurancaapk.ui.theme.SistemaSegurancaApkTheme
import com.google.firebase.database.FirebaseDatabase
import com.google.firebase.database.DataSnapshot
import com.google.firebase.database.DatabaseError
import com.google.firebase.database.ValueEventListener

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContent {
            SistemaSegurancaApkTheme {
                Scaffold(modifier = Modifier.fillMaxSize()) { innerPadding ->
                    ButtonScreen(modifier = Modifier.padding(innerPadding))
                }
            }
        }
    }
}

fun toggleLampada() {
    val database = FirebaseDatabase.getInstance()
    val lampadaRef = database.getReference("lampada")

    lampadaRef.addListenerForSingleValueEvent(object : ValueEventListener {
        override fun onDataChange(snapshot: DataSnapshot) {
            val currentValue = snapshot.getValue(Int::class.java) ?: 0
            val newValue = if (currentValue == 0) 1 else 0
            lampadaRef.setValue(newValue)
        }

        override fun onCancelled(error: DatabaseError) {
            // Tratar erros se necessário
        }
    })
}

@Composable
fun ButtonScreen(modifier: Modifier = Modifier) {
    Column(
        modifier = modifier
            .fillMaxSize()
            .padding(16.dp),
        verticalArrangement = Arrangement.Center,
        horizontalAlignment = Alignment.CenterHorizontally
    ) {
        Button(onClick = { /* TODO: Add action */ }) {
            Text("Ligar Buzzer")
        }
        Spacer(modifier = Modifier.height(16.dp)) // Espaço entre os botões
        Button(onClick = { toggleLampada() }) {
            Text("Ligar Lâmpada")
        }
        Spacer(modifier = Modifier.height(16.dp)) // Espaço entre os botões
        Button(onClick = { /* TODO: Add action */ }) {
            Text("Ligar Ventilador")
        }
    }
}

@Preview(showBackground = true)
@Composable
fun ButtonScreenPreview() {
    SistemaSegurancaApkTheme {
        ButtonScreen()
    }
}
