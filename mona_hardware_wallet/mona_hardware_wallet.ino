#include <EEPROM.h>
/**
 * FA***:アドレス  ,FL***:サイズ
 * |アドレス|プライベートキー|パスワード|チェック|
 * |FAAddress
 *          |FAPrivate
 *                           |FAPass
 *                                      |FAInitChek
 *                                               |FAEND
 */
#define FLAddress 34   //Address
#define FLPrivate 51   //PrivateKey
#define FLPass 6       //Password
#define FLInitChek 1   //InitializeChek

#define FAAddress 0
#define FAPrivate (FAAddress+FLAddress)
#define FAPass (FAPrivate+FLPrivate)
#define FAInitChek (FAPass+FLPass)
#define FAEND (FAInitChek+FLInitChek)

int Seq = 0;
String Adrs;
String Prky;
String Pswd;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
  }
  //initialize(); /*初回起動時実行してください*/
  initialcheck();
  initialMes();
}

void loop() {
  switch (Seq) {
    case 0:
      while (Serial.available() == 0) {
      }
      switch (Serial.read()) {
        case '1':
          Serial.println(F("Show Address"));
          Serial.println(F("Address is"));
          Serial.println(EEPROMread(FAAddress, FLAddress));
          initialMes2();
          break;

        case '2':
          Seq = 1;
          break;

        case '3':
          Seq = 2;
          break;

      }

      break;

    case 1:
      Serial.println(F("Show Private Key"));
      if (isPaswdCorrect(EEPROMread(FAPass, FLPass)) == true) {
        Serial.println(F("Private Key is"));
        Serial.println(EEPROMread(FAPrivate, FLPrivate));
      }
      Seq = 0;
      initialMes2();
      break;

    case 2:
      Serial.println(F("Setting"));
      if (isPaswdCorrect(EEPROMread(FAPass, FLPass)) == true) {
        Serial.println(F("1.Set Address and Private Key\n2.Set Password\n3.Initialize\n4.back"));
        while (Serial.available() == 0) {
        }
        switch (Serial.read()) {
          case '1':
            char inAdrs[34];
            char inPrky[51];
            Adrs = "";
            Prky = "";
            Serial.println(F("Set Address and Private Key"));
            Serial.println(F("Enter your NEW Address"));
            while (Serial.available() == 0) {
            }
            delay(500);
            for (int i = 0; i <= 33; i++) {
              inAdrs[i] = Serial.read();
            }
            for (int i = 0; i <= 33; i++) {
              Adrs.concat(inAdrs[i]);
            }
            Serial.println(F("Enter your NEW Private Key"));
            while (Serial.available() == 0) {
            }
            delay(500);
            for (int i = 0; i <= 50; i++) {
              inPrky[i] = Serial.read();
            }
            for (int i = 0; i <= 50; i++) {
              Prky.concat(inPrky[i]);
            }
            EEPROMwrite(FAAddress, Adrs);
            EEPROMwrite(FAPrivate, Prky);
            Serial.println(F("Set Address and Private Key Completed"));
            Seq = 0;
            initialMes2();
            break;

          case '2':
            Serial.println(F("Set Password"));
            Serial.println(F("Enter your NEW Password\nYou must use 6 character\nYou can use 0-9 A-Z a-z"));
            char inPswd[6];
            Pswd = "";
            while (Serial.available() == 0) {
            }
            delay(500);
            for (int i = 0; i <= 5; i++) {
              inPswd[i] = Serial.read();
            }
            for (int i = 0; i <= 5; i++) {
              Pswd.concat(inPswd[i]);
            }
            EEPROMwrite(FAPass, Pswd);
            Serial.println(F("Set Password Completed"));
            Seq = 0;
            initialMes2();

            break;

          case '3':
            Serial.println(F("Initialize?\n1.No\n2.Yes"));
            while (Serial.available() == 0) {
            }
            switch (Serial.read()) {
              case '1':
                Serial.println(F("Initialization was canceled"));
                Seq = 0;
                initialMes2();
                break;
              case '2':
                Serial.println(F("Are you sure?\n1.No\n2.Yes"));
                while (Serial.available() == 0) {
                }
                switch (Serial.read()) {
                  case '1':
                    Serial.println(F("Initialization was canceled"));
                    Seq = 0;
                    initialMes2();
                    break;
                  case '2':
                    Serial.println(F("Initializing..."));
                    initialize();
                    Serial.println(F("Initialization complete"));
                    Seq = 0;
                    initialMes2();
                    break;
                }
                break;
            }
            break;

          case '4':
            Seq = 0;
            initialMes2();
            break;
        }
        break;
      } else {
        initialMes2();
        Seq = 0;
      }
  }
}

void initialcheck() {
  String Intsum = "1";
  if (EEPROMread(FAInitChek, FLInitChek) != Intsum) {
    initialize();
  }
}

void initialize() {
  String IntAdrs = "InitialAddressXXXXXXXXXXXXXXXXXXXX";
  String IntPrky = "InitialPrivateKeyXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
  String IntPswd = "000000";
  String Intsum = "1";
  EEPROMwrite(FAAddress, IntAdrs);
  EEPROMwrite(FAPrivate, IntPrky);
  EEPROMwrite(FAPass, IntPswd);
  EEPROMwrite(FAInitChek, Intsum);
}

void initialMes() {
  initialMes1();
  initialMes2();
}

void initialMes1() {
  Serial.println(F("Welcome to Monacoin Hardware Wallet."));
}

void initialMes2() {
  Serial.println(F("MENU\n1.Show Address\n2.Show Private Key\n3.Setting"));
}

boolean isPaswdCorrect(String inpswd) {
  boolean correct;
  char key[6];
  String Pswdinput;
  Serial.println(F("Please enter password"));
  while (Serial.available() == 0) {
  }
  delay(500);
  for (int i = 0; i <= 5; i++) {
    key[i] = Serial.read();
  }
  for (int i = 0; i <= 5; i++) {
    Pswdinput.concat(key[i]);
  }
  if (Pswdinput == inpswd) {
    Serial.println(F("Password is correct"));
    correct = true;
  } else {
    Serial.println(F("Password is not correct"));
    correct = false;
  }
  return correct;
}

String EEPROMread (int adrs, int lng) {
  String readout;
  for (int i = 0; i < lng; i++) {
    readout.concat(char(EEPROM.read(adrs + i)));
  }
  return readout;
}

void EEPROMwrite (int adrs, String writein) {
  int lng = writein.length();
  for (int i = 0; i < lng; i++) {
    EEPROM.write(i + adrs,  writein.charAt(i));
  }
}

