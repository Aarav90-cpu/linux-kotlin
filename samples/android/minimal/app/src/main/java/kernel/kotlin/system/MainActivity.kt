package kernel.kotlin.system

import android.app.Activity
import android.os.Bundle
import android.widget.TextView

class MainActivity : Activity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        
        val textView = TextView(this).apply {
            text = "Hello from Android inside Linux Kernel Build System!"
            textSize = 24f
        }
        setContentView(textView)
    }
}
