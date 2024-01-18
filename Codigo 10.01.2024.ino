#include <LiquidCrystal.h>
#include <Keypad.h>

LiquidCrystal lcd (7,8,9,10,11,12);

//----------------Variaveis------------------
String gabarito = "12A45B";
String gabaritoADM = "402404";
String senha = "";
int comparaGabarito;
int comparaGabaritoADM;
int buzz = 13;
int dig = 0;
int LCDbrightness = 1023;
const unsigned long timeoutDuration = 10000;  // tempo ligado
unsigned long lastKeyPressTime = 0; //ultima vez de tecla pressionada
//-------------------------------------------

//---------------------------Variaveis do teclado----------------------------------------
const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns

char keys[ROWS][COLS] =
{
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','-','D'}
};

byte rowPins[ROWS] = {A5, A4, A3, A2}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {3, 4, 5, 6}; //connect to the column pinouts of the keypad
//---------------------------------------------------------------------------------------

//----------------------------Char-------------------------------------------------------
byte lamp[8]=
{B00000, B00011, B00100, B00100, B00100, B00011, B00011, B00011};

byte LP[8]=
{B10000, B10100, B10100, B10110, B10000, B10110, B10110, B10100};

byte CF[8]=
{B00000, B01110, B10001, B10001, B11111, B11011, B11011, B11111};

byte CA[8]=
{B01110, B10001, B10001, B00001, B01111, B11011, B11011, B11111};

byte novo[8]=
{B00111, B01001, B10000, B10010, B10111, B10010, B10000, B11111};
//---------------------------------------------------------------------------------------

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void impressao(char caracter)
{
  buzzer();
  lcd.setCursor(dig, 1);
  lcd.print(caracter);
  dig++;
}

void comparador(char keys)
{
  comparaGabarito = senha.compareTo(gabarito);
  comparaGabaritoADM = senha.compareTo(gabaritoADM);
  senha += keys;
}

void resetar()
{
  senha = "";
  delay(1000);
  lcd.home();
  lcd.write(byte(3));
  lcd.print(" Senha:      ");
  lcd.setCursor(0, 1);
  lcd.print("                ");
  dig = 0;
}

void abrir()
{
  digitalWrite(2, LOW);
  delay(500);
  digitalWrite(2, HIGH);
}

void buzzer()
{
  tone(buzz, 1568);
  delay(150);
  noTone(buzz);
}

void zelda()
{
  tone(buzz, 1568);
  delay(200);
  tone(buzz, 1481.5);
  delay(200);
  tone(buzz, 1246.6);
  delay(200);
  tone(buzz, 880);
  delay(200);
  tone(buzz, 826);
  delay(200);
  tone(buzz, 1318.5);
  delay(200);
  tone(buzz, 1664);
  delay(150);
  tone(buzz, 2093);
  delay(250);
  noTone(buzz);
}

//---------------------------------------------*Setup*-------------------------------------------------------------
void setup()
{
  pinMode(buzz, OUTPUT);
  pinMode(2 , OUTPUT);
  pinMode(A1, OUTPUT); 
  
  digitalWrite(2, HIGH);
  analogWrite(A1, LCDbrightness);

  lcd.begin(2, 16);
  lcd.createChar(1, lamp);
  lcd.createChar(2, LP);
  lcd.createChar(3, CF);
  lcd.createChar(4, CA);
  lcd.createChar(5, novo);
  lcd.home();
  lcd.write(byte(3));
  lcd.print(" Senha:      ");
  lcd.write(byte(1));
  lcd.write(byte(2));
  lcd.setCursor(0,1);
}

void loop()
{
  char key = keypad.getKey();
  retorno:
  

  if (key != NO_KEY)
  {
    lastKeyPressTime = millis();
    analogWrite(A1, LCDbrightness);
    comparador(key);
    impressao('*');

    if (senha.length() > 10)
    {
      resetar();
      buzzer();
      key = 0;
      goto retorno;
    }
    
    switch(key)
    {
      case '-':
        if (comparaGabarito == 0)
        {
          lcd.home();
          lcd.write(byte(4));
          lcd.setCursor(0, 1);
          lcd.print("Bem-Vindo       ");
          abrir();
          zelda();
          resetar();
        }
        else if (comparaGabaritoADM == 0)
        {
          resetar();
          lcd.home();
          lcd.write(byte(5));
          lcd.setCursor(1, 0);
          lcd.print(" Nova Senha:");
          lcd.setCursor(0, 1);
          key = 0;
          while (dig < 6)
          {
            char key = keypad.getKey();
            if (key != NO_KEY)
            {
              gabarito.setCharAt(dig, key);
              impressao(key);
              key = 0;
            }
          }
          resetar();
        } 
        else
        {
          lcd.setCursor(0, 1);
          lcd.print("Senha Incorreta ");
          resetar();
        }
        key = 0;
        goto retorno;

      case '*':
        resetar();
        buzzer();
        key = 0;
        goto retorno;

      default:
        key = 0;
        goto retorno;
    }
  }

  if (millis() - lastKeyPressTime > timeoutDuration)
  {
    analogWrite(A1, 0);
  }
}