package com.example.calculator;

import android.annotation.SuppressLint;
import android.os.Bundle;
import android.text.InputType;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;
import com.example.calculator.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {
    private static final String TAG = "main";
    private StringBuilder sb = new StringBuilder();
    private String displayString = "0";
    private boolean allowPoint = false;
    private boolean allowOp = false;

    // Used to load the 'calculator' library on application startup.
    static {
        System.loadLibrary("calculator");
    }


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        com.example.calculator.databinding.ActivityMainBinding binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());
        setEventListener(binding);
        binding.screen.setText(displayString);
    }

    /**
     * A native method that is implemented by the 'calculator' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI(String displayStr);

    private void setEventListener(ActivityMainBinding binding) {
        binding.one.setOnClickListener(new NumberClickListener(binding, 1));
        binding.two.setOnClickListener(new NumberClickListener(binding, 2));
        binding.three.setOnClickListener(new NumberClickListener(binding, 3));
        binding.four.setOnClickListener(new NumberClickListener(binding, 4));
        binding.five.setOnClickListener(new NumberClickListener(binding, 5));
        binding.six.setOnClickListener(new NumberClickListener(binding, 6));
        binding.seven.setOnClickListener(new NumberClickListener(binding, 7));
        binding.eight.setOnClickListener(new NumberClickListener(binding, 8));
        binding.nine.setOnClickListener(new NumberClickListener(binding, 9));
        binding.zero.setOnClickListener(new NumberClickListener(binding, 0));
        binding.add.setOnClickListener(new OpClickListener(binding, '+'));
        binding.reduce.setOnClickListener(new OpClickListener(binding, '-'));
        binding.ride.setOnClickListener(new OpClickListener(binding, '×'));
        binding.divide.setOnClickListener(new OpClickListener(binding, '÷'));
        binding.point.setOnClickListener(new OpClickListener(binding, '.'));
        binding.leftBracket.setOnClickListener(new OpClickListener(binding, '('));
        binding.rightBracket.setOnClickListener(new OpClickListener(binding, ')'));
        binding.equal.setOnClickListener(new OpClickListener(binding, '='));
        binding.clr.setOnClickListener(new OpClickListener(binding, 'X'));
    }

    class NumberClickListener implements View.OnClickListener {
        com.example.calculator.databinding.ActivityMainBinding binding;
        int num;
        NumberClickListener(com.example.calculator.databinding.ActivityMainBinding binding, int num) {
            super();
            this.binding = binding;
            this.num = num;
        }
        @Override
        public void onClick(View view) {
            sb.append(num);
            allowPoint = true;
            allowOp = true;
            displayString = sb.toString();
            binding.screen.setText(displayString);
        }
    }

    class OpClickListener implements View.OnClickListener {
        com.example.calculator.databinding.ActivityMainBinding binding;
        char op;
        OpClickListener(com.example.calculator.databinding.ActivityMainBinding binding, char op) {
            super();
            this.binding = binding;
            this.op = op;
        }
        @Override
        public void onClick(View view) {
            switch (op) {
                case '.':
                    if (allowPoint) {
                        sb.append(".");
                        allowPoint = false;
                        allowOp = false;
                    }
                    break;
                case '+':
                case '-':
                case '×':
                case '÷':
                    if (allowOp) {
                        sb.append(op);
                        allowOp = false;
                        allowPoint = false;
                    }
                    break;
                case '(':
                    sb.append(op);
                    allowOp = false;
                    allowPoint = false;
                    break;
                case ')':
                    sb.append(op);
                    allowPoint = false;
                    break;
                case 'X':
                    sb.delete(0, sb.length());
                    allowPoint = false;
                    allowOp = false;
                    break;
                case '=':
                    displayString = stringFromJNI(sb.toString().replace('×', '*').replace('÷', '/'));
                    binding.screen.setText(displayString);
                    sb.delete(0, sb.length());
                    // Toast.makeText(MainActivity.this, displayString, Toast.LENGTH_SHORT).show();
                    return;
            }
            displayString = sb.length() > 0 ? sb.toString() : "0";
            binding.screen.setText(displayString);
        }
    }
}