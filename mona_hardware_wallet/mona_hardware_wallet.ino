#include <EEPROM.h>

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
          Serial.println("Show Address");
          Serial.println("Address is");
          Serial.println(EEPROMread(0, 33));
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
      Serial.println("Show Private Key");
      if (isPaswdCorrect(EEPROMread(85, 6)) == true) {
        Serial.println("Private Key is");
        Serial.println(EEPROMread(34, 51));
      }
      Seq = 0;
      initialMes2();
      break;

    case 2:
      Serial.println("Setting");
      if (isPaswdCorrect(EEPROMread(85, 6)) == true) {
        Serial.println("1.Set Address and Private Key\n2.Set Password\n3.Initialize\n4.back");
        while (Serial.available() == 0) {
        }
        switch (Serial.read()) {
          case '1':
            char inAdrs[34];
            char inPrky[51];
            Adrs = "";
            Prky = "";
            Serial.println("Set Address and Private Key");
            Serial.println("Enter your NEW Address");
            while (Serial.available() == 0) {
            }
            delay(500);
            for (int i = 0; i <= 33; i++) {
              inAdrs[i] = Serial.read();
            }
            for (int i = 0; i <= 33; i++) {
              Adrs.concat(inAdrs[i]);
            }
            Serial.println("Enter your NEW Private Key");
            while (Serial.available() == 0) {
            }
            delay(500);
            for (int i = 0; i <= 50; i++) {
              inPrky[i] = Serial.read();
            }
            for (int i = 0; i <= 50; i++) {
              Prky.concat(inPrky[i]);
            }
            EEPROMwrite(0, Adrs);
            EEPROMwrite(34, Prky);
            Serial.println("Set Address and Private Key Completed");
            Seq = 0;
            initialMes2();
            break;

          case '2':
            Serial.println("Set Password");
            Serial.println("Enter your NEW Password\nYou must use 6 character\nYou can use 0-9 A-Z a-z");
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
            EEPROMwrite(85, Pswd);
            Serial.println("Set Password Completed");
            Seq = 0;
            initialMes2();

            break;

          case '3':
            Serial.println("Initialize?\n1.No\n2.Yes");
            while (Serial.available() == 0) {
            }
            switch (Serial.read()) {
              case '1':
                Serial.println("Initialization was canceled");
                Seq = 0;
                initialMes2();;
                break;
              case '2':
                Serial.println("Are you sure?\n1.No\n2.Yes");
                while (Serial.available() == 0) {
                }
                switch (Serial.read()) {
                  case '1':
                    Serial.println("Initialization was canceled");
                    Seq = 0;
                    initialMes2();
                    break;
                  case '2':
                    Serial.println("Initializing...");
                    initialize();
                    Serial.println("Initialization complete");
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
  if (EEPROMread(91, 1) != Intsum) {
    initialize();
  }
}

void initialize() {
  String IntAdrs = "InitialAddressXXXXXXXXXXXXXXXXXXXX";
  String IntPrky = "InitialPrivateKeyXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
  String IntPswd = "000000";
  String Intsum = "1";
  EEPROMwrite(0, IntAdrs);
  EEPROMwrite(34, IntPrky);
  EEPROMwrite(85, IntPswd);
  EEPROMwrite(91, Intsum);
}

void initialMes() {
  initialMes1();
  initialMes2();
}

void initialMes1() {
  Serial.println("Welcome to Monacoin Hardware Wallet.");
}

void initialMes2() {
  Serial.println("MENU\n1.Show Address\n2.Show Private Key\n3.Setting");
}

boolean isPaswdCorrect(String inpswd) {
  boolean correct;
  char key[6];
  String Pswdinput;
  Serial.println("Please enter password");
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
    Serial.println("Password is correct");
    correct = true;
  } else {
    Serial.println("Password is not correct");
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

