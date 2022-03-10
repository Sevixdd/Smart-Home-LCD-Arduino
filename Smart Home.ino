
#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

#define YELLOW 0x3
#define TEAL 0x6
#define WHITE 0x7

//Arrays
String mainMenu[] = {"FIRST", "GROUND", "OUTSIDE","SERIAL PRINT"};
String firstFloor[] = {"BEDROOM1", "BEDROOM2", "BATHROOM"};
String groundFloor[] = {"KITCHEN", "HALL","LIVING"};
String outside[] = {"GARAGE", "GARDEN"};
String bedroomUtilities[]={"LIGHT","HEAT","LAMP"};
String basicUtilities[]={"LIGHT","HEAT"};
String garden[]={"LIGHT","WATER"};
String garage[]={"LAMP","LIGHT"};
String all[19]; // there is stored the data for read
// Navigation button variables
int readKey;

// Menu control variables
int menuPage = 0;
int maxMainPages = 2;
int maxThreePages = 1;
int maxOutsidePages = 0;
int cursorPos = 0;
int roomsCounter; // this variable is used for the rooms
int floorCounter; // this variable is used for Firs floor, Ground floor and Outside
int Utilities; //this variable is used for Light, Lamp, Water and Heat

  //custom arrows
  byte upArrow[8] = { 0x04,0x0E,0x1B,0x11,0x00,0x00,0x00,0x00};
  byte downArrow[8] = {0x00,0x00,0x00,0x00,0x11,0x1B,0x0E,0x04};
  byte menuCursor[8] = { 0x00,0x04,0x06,0x1F, 0x06,0x04,0x00,0x00};
  byte LeftArrow[8] = {0x00,0x04,0x0C,0x1F,0x0C,0x04,0x00,0x00};
  byte RightArrow[8] = {0x00,0x04,0x06,0x1F,0x06,0x04,0x00,0x00};

#ifdef __arm__
extern "C" char* sbrk(int incr);
#else // __ARM__
extern char *__brkval;
#endif // __arm__
int freeMemory() { // (taken from lectures from https://github.com/mpflaga/ArduinoMemoryFree):
char top;
#ifdef __arm__
return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
return &top - __brkval;
#else // __arm__
return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif // __arm__
}


void setup() {
  Serial.begin(9600); 
  Serial.println("ENHANCED: LAMP, OUTSIDE, QUERY, MEMORY\n");
  lcd.begin(16, 2); // set up the LCD's number of columns and rows:
  lcd.createChar(0, menuCursor); 
  lcd.createChar(1, upArrow);
  lcd.createChar(2, downArrow);
  lcd.createChar(3, LeftArrow);
  lcd.createChar(4, RightArrow);
  serialPrintCreate();
  moreThanThreePagesDraw(mainMenu,maxMainPages);
  drawCursor();
  operateMainMenu();
}
void loop() {
  if (Serial.available()) { // Query funcitonality in order to Serial print data, after Q is entered you need to press a button
    char Query = Serial.read();
    if(Query == 'Q')
      serialPrint();
    if(Query == 'M')
      {
        Serial.println("The amount of RAM memory unused is: ");
        Serial.println(freeMemory());
      }
    Query = ' ';
   }
  if(roomsCounter == 0){
    
    lcd.clear();
    moreThanThreePagesDraw(mainMenu,maxMainPages);
    drawCursor();
    operateMainMenu();
  }
  else if (roomsCounter == 1){
    lcd.clear();
    moreThanThreePagesDraw(firstFloor,maxThreePages );
    drawCursor();
    First();
  }
  else if (roomsCounter == 2){
    lcd.clear();
    moreThanThreePagesDraw(groundFloor,maxThreePages);
    drawCursor();
    Ground();
  }
  else if (roomsCounter == 3){
    lcd.clear();
    twoPagesMenuDraw(outside);
    drawCursor();
    Outside();
  }
  else if (roomsCounter == 4){
    if(Utilities == 0)
      {
        moreThanThreePagesDraw(bedroomUtilities,maxThreePages);
        drawCursor();
        bedroom1();
      }
    if(Utilities == 1)
      {
        lcd.clear();
        lcd.setBacklight(TEAL);
        drawCursor();
        lightLampBedroom(1);
      }
     if(Utilities == 2)
      {
        lcd.clear();
        lcd.setBacklight(TEAL);
        drawCursor();
        lightLampBedroom(3);
      }
     if(Utilities == 12)
     {
        lcd.clear();
        lcd.setBacklight(YELLOW);
        drawCursor();
        heatBedroom(2);
     }
  }
  else if (roomsCounter == 5){
    if(Utilities == 0)
    {
      lcd.clear();
      moreThanThreePagesDraw(bedroomUtilities,maxThreePages);
      drawCursor();
      bedroom2();
    }
    if(Utilities == 3)
      {
        lcd.clear();
        lcd.setBacklight(TEAL);
        drawCursor();
        lightLampBedroom(4);
      }
     if(Utilities == 4)
      {
        lcd.clear();
        lcd.setBacklight(TEAL);
        drawCursor();
        lightLampBedroom(6);
      }
      if(Utilities == 13)
      {
        lcd.clear();
        lcd.setBacklight(YELLOW);
        drawCursor();
        heatBedroom(5);
      }
  }
  else if (roomsCounter == 6){
    if(Utilities == 0)
    {
      lcd.clear();
      twoPagesMenuDraw(basicUtilities);
      drawCursor();
      bathroom();
    }
    if(Utilities == 5)
      {
        lcd.clear();
        lcd.setBacklight(TEAL);
        drawCursor();
        lightTwoPages(7);
      }
      if(Utilities == 14)
      {
        lcd.clear();
        lcd.setBacklight(YELLOW);
        drawCursor();
        heatBedroom(8);
      }
    
  }
  else if (roomsCounter == 7){
    if(Utilities == 0)
    {
      lcd.clear();
      twoPagesMenuDraw(basicUtilities);
      drawCursor();
      kitchen();
    }
    if(Utilities == 6)
      {
        lcd.clear();
        lcd.setBacklight(TEAL);
        drawCursor();
        lightTwoPages(9);
      }
     if(Utilities == 15)
      {
        lcd.clear();
        lcd.setBacklight(YELLOW);
        drawCursor();
        heatBedroom(10);
      }
  }
  else if (roomsCounter == 8){
    if(Utilities == 0)
    {
      lcd.clear();
      twoPagesMenuDraw(basicUtilities);
      drawCursor();
      hall();
    }
    if(Utilities == 7)
      {
        lcd.clear();
        lcd.setBacklight(TEAL);
        drawCursor();
        lightTwoPages(11);
      }
      if(Utilities == 16)
      {
        lcd.clear();
        lcd.setBacklight(YELLOW);
        drawCursor();
        heatBedroom(12);
      }
  }
  else if (roomsCounter == 9){
    if(Utilities == 0)
    {
      lcd.clear();
      twoPagesMenuDraw(basicUtilities);
      drawCursor();
      living();
    }
    if(Utilities == 8)
      {
        lcd.clear();
        lcd.setBacklight(TEAL);
        drawCursor();
        lightTwoPages(13);
      }
      if(Utilities == 17)
      {
        lcd.clear();
        lcd.setBacklight(YELLOW);
        drawCursor();
        heatBedroom(14);
      }
    
  }
  else if (roomsCounter == 10){
    if(Utilities == 0)
    {
      lcd.clear();
      twoPagesMenuDraw(garage);
      drawCursor();
      garageOutside();
    }
    if(Utilities == 9)
      {
        lcd.clear();
        lcd.setBacklight(TEAL);
        drawCursor();
        lightTwoPages(15);
      }
      if(Utilities == 10)
      {
        lcd.clear();
        lcd.setBacklight(TEAL);
        drawCursor();
        lightTwoPages(16);
      }
    
    
  }
  else if (roomsCounter == 11){
    if(Utilities == 0)
    {
      lcd.clear();
      twoPagesMenuDraw(garden);
      drawCursor();
      gardenOutside();
    }
    if(Utilities == 11)
      {
        lcd.clear();
        lcd.setBacklight(TEAL);
        drawCursor();
        lightTwoPages(17);
      }
      if(Utilities == 18)
      {
        lcd.clear();
        lcd.setBacklight(TEAL);
        drawCursor();
        lightTwoPages(18);
      }
    
  }
  
  
   
}

//This function draws 2 menu items on the screen is used to render 3 or more items page
void moreThanThreePagesDraw(String items[],int maxPages) {
  lcd.setBacklight(WHITE);
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(items[menuPage]);
  lcd.setCursor(1, 1);
  lcd.print(items[menuPage + 1]);
  if (menuPage == 0) {
    lcd.setCursor(15, 1);
    lcd.write(byte(2));
  } else if (menuPage > 0 and menuPage < maxPages) {
    lcd.setCursor(15, 1);
    lcd.write(byte(2));
    lcd.setCursor(15, 0);
    lcd.write(byte(1));
  } else if (menuPage == maxPages) {
    lcd.setCursor(15, 0);
    lcd.write(byte(1));
  }
}

//This function draws 2 menu items on the screen is used to render a 2 items page
void twoPagesMenuDraw(String items[]) {
  lcd.setBacklight(WHITE);
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(items[menuPage]);
  lcd.setCursor(1, 1);
  lcd.print(items[menuPage + 1]);
}


//This function draws 2 menu items on the screen is used to render 3 or more items page with levels
void heatPageDraw(String items[],int maxPages,String heat) {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(items[menuPage]);
  lcd.setCursor(1, 1);
  lcd.print(items[menuPage + 1]);
      lcd.setCursor(10, 0);
      lcd.print(heat);
      lcd.setCursor(9, 0);
      lcd.write(byte(3));
      lcd.setCursor(11, 0);
      lcd.write(byte(4));
  if (menuPage == 0) {
    lcd.setCursor(15, 1);
    lcd.write(byte(2));
  } else if (menuPage > 0 and menuPage < maxPages) {
    lcd.setCursor(15, 1);
    lcd.write(byte(2));
    lcd.setCursor(15, 0);
    lcd.write(byte(1));
  } else if (menuPage == maxPages) {
    lcd.setCursor(15, 0);
    lcd.write(byte(1));
  }
}

//This function draws 2 menu items on the screen is used to render a 2 items page with levels

void heatTwoPageDraw(String items[] , String heat) {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(items[menuPage]);
  lcd.setCursor(1, 1);
  lcd.print(items[menuPage + 1]);
      lcd.setCursor(10, 0);
      lcd.print(heat);
      lcd.setCursor(9, 0);
      lcd.write(byte(3));
      lcd.setCursor(11, 0);
      lcd.write(byte(4));
  
}

//This function is used to render a 3 or more items page with on/off 

void OnOffPagesDraw(String items[],int maxPages,int on) {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(items[menuPage]);
  lcd.setCursor(1, 1);
  lcd.print(items[menuPage + 1]);
  if(on == 1)
    {
      lcd.setCursor(8, 0);
      lcd.print("OFF");
      lcd.setCursor(7, 0);
      lcd.write(byte(3));
      lcd.setCursor(11, 0);
      lcd.write(byte(4));
    }
  if(on == 2)
    {
      lcd.setCursor(8, 0);
      lcd.print("ON");
      lcd.setCursor(7, 0);
      lcd.write(byte(3));
      lcd.setCursor(10, 0);
      lcd.write(byte(4));
    }
  if (menuPage == 0) {
    lcd.setCursor(15, 1);
    lcd.write(byte(2));
  } else if (menuPage > 0 and menuPage < maxPages) {
    lcd.setCursor(15, 1);
    lcd.write(byte(2));
    lcd.setCursor(15, 0);
    lcd.write(byte(1));
  } else if (menuPage == maxPages) {
    lcd.setCursor(15, 0);
    lcd.write(byte(1));
  }
}

//This function is used to render a 2 items page with on/off 
void OnOffTwoPagesDraw(String items[],int on) {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(items[menuPage]);
  lcd.setCursor(1, 1);
  lcd.print(items[menuPage + 1]);
  if(on == 1)
    {
      lcd.setCursor(8, 0);
      lcd.print("OFF");
      lcd.setCursor(7, 0);
      lcd.write(byte(3));
      lcd.setCursor(11, 0);
      lcd.write(byte(4));
    }
  if(on == 2)
    {
      lcd.setCursor(8, 0);
      lcd.print("ON");
      lcd.setCursor(7, 0);
      lcd.write(byte(3));
      lcd.setCursor(10, 0);
      lcd.write(byte(4));
    }
}


// When called, this function will erase the current cursor and redraw it based on the cursorPos and menuPage variables.
void drawCursor() {
  for (int i = 0; i <= 2; i++) {  // Erases current cursor
    lcd.setCursor(0, i);
    lcd.print(" ");
  }

  // in order to determine where the cursor should be you need to see if you are at an odd or even menu page and an odd or even cursor position.
  if (menuPage % 2 == 0) {
    if (cursorPos % 2 == 0) {  // if the menu page is even and the cursor position is even that means the cursor should be on line 1
      lcd.setCursor(0, 0);
      lcd.write(byte(0));
    }
    else if (cursorPos % 2 != 0) {  // if the menu page is even and the cursor position is odd that means the cursor should be on line 2
      lcd.setCursor(0, 1);
      lcd.write(byte(0));
    }
  }
  if (menuPage % 2 != 0) {
    if (cursorPos % 2 == 0) {  // if the menu page is odd and the cursor position is even that means the cursor should be on line 2
      lcd.setCursor(0, 1);
      lcd.write(byte(0));
    }
    else if (cursorPos % 2 != 0) {  // if the menu page is odd and the cursor position is odd that means the cursor should be on line 1
      lcd.setCursor(0, 0);
      lcd.write(byte(0));
    }
  }
}


//This is the main menu functionality, its used with 4 buttons left(back), up(cursor up), down(cursor down), select 
void operateMainMenu() {
   
  moreThanThreePagesDraw(mainMenu,maxMainPages);
  drawCursor();
  int going = 0;
  int button;
  while (going == 0) {
    
    readKey = lcd.readButtons();
    if (readKey < 6) {
      delay(50);
      readKey = lcd.readButtons();
    }
    button = readButtons(readKey);
    switch (button) {
      case 0: // When button returns as 0 there is no action taken
        break;
      case 1:  // this case is select button
        button = 0;
        switch (cursorPos) { // The case that is selected here is dependent on which menu page you are on and where the cursor is.
          case 0: //1st item
            {
              menuPage = 0;
              cursorPos = 0;
              roomsCounter = 1; //will start first floor function
            }
            break;
          case 1: //2nd item
            {
              menuPage=0;
              cursorPos=0;
              roomsCounter=2; //will start ground floor function
            }
            break;
          case 2: //3rd item
            {
              menuPage = 0;
              cursorPos = 0;
              roomsCounter = 3; //will start outside function
            }
            break;
            case 3: // 4th item will print the menu to Serial
            {
              menuPage = 0;
              cursorPos =0 ;
              Serial.println();
              serialPrint();
            }
            break;
        }
        going = 1;
        break;
      case 2: // cursor will move up 1 item
        button = 0;
        cursorUp(mainMenu,((sizeof(mainMenu) / sizeof(String)) - 1),maxMainPages);
        moreThanThreePagesDraw(mainMenu,maxMainPages);
        drawCursor();
        going = 1;
        break;
      case 3: // cursor will move down 1 item
        button = 0;
        cursorDown(mainMenu,((sizeof(mainMenu) / sizeof(String)) - 1),maxMainPages);
        moreThanThreePagesDraw(mainMenu,maxMainPages);
        drawCursor();
        going = 1;
        break;
      case 4: 
        button = 0;
        going = 1;
        break;
    }
  }
}
// this function is used to move the cursor up
void cursorUp(String items[],size_t len, int maxMenuPages){
   if (menuPage == 0) {
          cursorPos = cursorPos - 1;
          cursorPos = constrain(cursorPos, 0, len);
        }
   else if (menuPage % 2 == 0 and cursorPos % 2 == 0) {
          menuPage = menuPage - 1;
          menuPage = constrain(menuPage, 0, maxMenuPages);
        }
   else if (menuPage % 2 != 0 and cursorPos % 2 != 0) {
          menuPage = menuPage - 1;
          menuPage = constrain(menuPage, 0, maxMenuPages);
        }
        cursorPos = cursorPos - 1;
        cursorPos = constrain(cursorPos, 0, len);
}
// this function is used to move the cursor down
void cursorDown(String items[],size_t len, int maxMenuPages){
   if (menuPage % 2 == 0 and cursorPos % 2 != 0) {
          menuPage = menuPage + 1;
          menuPage = constrain(menuPage, 0, maxMenuPages);
        }
   else if (menuPage % 2 != 0 and cursorPos % 2 == 0) {
          menuPage = menuPage + 1;
          menuPage = constrain(menuPage, 0, maxMenuPages);
        }
        cursorPos = cursorPos + 1;
        cursorPos = constrain(cursorPos, 0, len);
}
// This function is used to get values for the button pressed
int readButtons(int x) {
  byte buttonRead = lcd.readButtons();
  int result = 0;
  if (buttonRead & BUTTON_SELECT) {
    result = 1; // select
  } else if (buttonRead & BUTTON_UP) {
    result = 2; // up
  } else if (buttonRead & BUTTON_DOWN) {
    result = 3; // down
  } else if (buttonRead & BUTTON_LEFT) {
    result = 4; // left
  } else if (buttonRead & BUTTON_RIGHT) {
    result = 5; // right
  }
  return result;
}

//This is the main functionality of the menu for 3 or more items, its used with 4 buttons left(back), up(cursor up), down(cursor down), select 
//It's called in every floor or room function (with 3 or more items)
void threeItemsMenu(String items[],size_t len,int i){
  
  int going = 0;
    int button;
  drawCursor();
  while (going == 0) {
    readKey = lcd.readButtons();
    if (readKey < 6) {
      delay(50);
      readKey = lcd.readButtons();
    }
    
    button = readButtons(readKey);
    switch (button) {
      case 0: // When button returns as 0 there is no action taken
        break;
      case 1:  // this case is select button
        button = 0;
        switch (cursorPos) { // The case that is selected here is dependent on which menu page you are on and where the cursor is.
          case 0:
            if(roomsCounter == 4)
              Utilities = 1; // will start on/off func for three pages
            if(roomsCounter == 5)
              Utilities = 3; // will start on/off func for three pages
            if(floorCounter == 1 && roomsCounter == 1)
               {
                menuPage = 0;
                cursorPos = 0;
                roomsCounter = 4; // will start bedroom1 function
               }
            else if(floorCounter == 2 )
               {
                menuPage = 0;
                cursorPos = 0;
                roomsCounter = 7; // will start kitchen function
               }
            break;
          case 1:
            if(roomsCounter == 4)
              Utilities = 12; // will start levels func for three pages
            else if(roomsCounter == 5)
              Utilities = 13; // will start levels func for three pages
            if(floorCounter == 1 && roomsCounter == 1)
              {
                menuPage = 0;
                cursorPos = 0;
                roomsCounter = 5; // will start bedroom2 function
              }
            else if(floorCounter == 2)
              {
                menuPage = 0;
                cursorPos = 0;
                roomsCounter = 8; // will start hall function
              }
            break;
          case 2:
            if(roomsCounter == 4)
                  Utilities = 2; // will start on/off func for three pages
            else if(roomsCounter==5)
                  Utilities = 4; // will start on/off func for three pages
            if(floorCounter == 1 && roomsCounter == 1)
              {
                menuPage = 0;
                cursorPos = 0;
                roomsCounter = 6; // will start bathroom function
              }
            else if(floorCounter == 2)
              {
                menuPage = 0;
                cursorPos = 0;
                roomsCounter = 9; // will start living function
              }
            break;
        }
        going = 1;
        moreThanThreePagesDraw(items,maxThreePages);
        drawCursor();
        break;
      case 2: // down button used to scroll
        button = 0;
        moreThanThreePagesDraw(items,maxThreePages);
        cursorUp(items,len,maxThreePages);
        drawCursor();
        going = 1;
        break;
      case 3: // down button used to scroll
        button = 0;
        moreThanThreePagesDraw(items,maxThreePages); 
        cursorDown(items,len,maxThreePages);
        drawCursor();
        going = 1;
        break;
      case 4:  // When left button is used as a back to previous screen button
        button = 0;
        going = 1;
        if(roomsCounter < 3) // If you are on FirstFloor/GroundFlorr/Outside when button is pressed you are going back to Main menu
          {
            menuPage = 0;
            cursorPos = 0;
            roomsCounter = 0;
          }
        else if(roomsCounter == 4 && floorCounter == 1 || roomsCounter == 5 && floorCounter == 1 || roomsCounter == 6 && floorCounter == 1) // If you are on Bedroom1/Bedroom2/Bathroom when button is pressed you are going back to first floor
          {
            menuPage = 0;
            cursorPos = 0;
            roomsCounter = 1;
          }
        else if(roomsCounter == 7 && floorCounter == 2 || roomsCounter == 8 && floorCounter == 2 || roomsCounter == 9 && floorCounter == 2) // If you are on Kitche/Hall/Living when button is pressed you are going back to Ground floor
          {
            menuPage = 0;
            cursorPos = 0;
            roomsCounter = 2;
          }
        else if(roomsCounter == 10) 
          {
            menuPage = 0;
            cursorPos = 0;
            roomsCounter = 3;
          }
        lcd.setBacklight(WHITE);
        break;
    }
  }
}

//This is the main functionality of the menu for 2 items, its used with 4 buttons left(back), up(cursor up), down(cursor down), select 
//It's called in every floor or room function (with 2 items)
void twoItemsMenu(String items[],size_t len,int i){
  
  int going = 0;
  int button;
  twoPagesMenuDraw(items);
  drawCursor();
  while (going == 0) {
    
    readKey = lcd.readButtons();
    if (readKey < 6) {
      delay(50);
      readKey = lcd.readButtons();
    }
    
    button = readButtons(readKey);
    switch (button) {
      case 0: // When button returns as 0 there is no action taken
        break;
      case 1:  // This case is select button
        button = 0;
        switch (cursorPos) { // The selected option is based on the position of the cursor
          case 0:
            if(roomsCounter == 10) //garage
                Utilities = 9; // Will start on/off func for two pages
            if(floorCounter == 3 && roomsCounter == 3)
              {
                menuPage = 0;
                cursorPos = 0;
                roomsCounter = 10;
              }
            if(roomsCounter == 6) //bathroom
                Utilities = 5; // will start on/off func for two pages
             else if(roomsCounter == 7) //kitchen
                Utilities = 6; // will start on/off func for two pages
             else if(roomsCounter == 8) //hall
                Utilities = 7; // will start on/off func for two pages
             else if(roomsCounter == 9) //living
                Utilities = 8; // will start on/off func for two pages
             else if(roomsCounter == 11)// garden
                Utilities = 11; // will start on/off func for two pages
            break;
          case 1:

            if(roomsCounter == 11)
                Utilities = 18; // Will start levels func for two pages
            if(floorCounter == 3 && roomsCounter == 3)
              {
                menuPage = 0;
                cursorPos = 0;
                roomsCounter = 11;
              }
             if(roomsCounter == 10)
                Utilities = 10; // Will start levels func for two pages
             if(roomsCounter == 5)
                Utilities = 13; // Will start levels func for two pages
             else if(roomsCounter == 6)
                Utilities = 14; // Will start levels func for two pages
             else if(roomsCounter == 7)
                Utilities = 15; // Will start levels func for two pages
             else if(roomsCounter == 8)
                Utilities = 16; // Will start levels func for two pages
             else if(roomsCounter == 9)
                Utilities = 17; // Will start levels func for two pages
             
            break;
        }
        going = 1;
        twoPagesMenuDraw(items);
        drawCursor();
        break;
      case 2:// up button used to scroll
        button = 0;
        twoPagesMenuDraw(items);
        cursorUp(items,len,maxOutsidePages);
        drawCursor();
        going = 1;
        break;
      case 3: // down button used to scroll
        button = 0;
        twoPagesMenuDraw(items);
        cursorDown(items,len,maxOutsidePages);
        drawCursor();
        going = 1;
        break;
      case 4:  // When left button is used as a back to previous screen button
        button = 0;
        going = 1;
        if(roomsCounter == 3) // If you are on outside it goes back to main menu
          {
            menuPage = 0;
            cursorPos = 0;
            roomsCounter = 0;
          }
        else if(roomsCounter == 7 || roomsCounter == 8 || roomsCounter == 9) // If you are on Kitche/Hall/Living when button is pressed you are going back to Ground floor
          {
            menuPage = 0;
            cursorPos = 0;
            roomsCounter = 2;
          }
        else if(roomsCounter == 6) // If you are on Bathrom when button is pressed you are going back to First floor
          {
            menuPage = 0;
            cursorPos = 0;
            roomsCounter = 1;
          }
         else if(roomsCounter == 10 || roomsCounter == 11) // if you are on Garage/Garden when button is pressed you are going back to Outisde
          {
            menuPage = 0;
            cursorPos = 0;
            roomsCounter = 3;
          }
        lcd.setBacklight(WHITE);
        break;
    }
  }
}


void First(){// First floor (1st item) executes when selected from mainMenu
  floorCounter = 1;//counter to use it in threeitemsMenu cases
  
  threeItemsMenu(firstFloor,((sizeof(firstFloor) / sizeof(String)) - 1),floorCounter);
}

void Ground() { // Ground floor (2nd item) executes when selected from mainMenu
  floorCounter = 2;//counter to use it in threeitemsMenu cases
  threeItemsMenu(groundFloor,((sizeof(groundFloor) / sizeof(String)) - 1),floorCounter);
}

void Outside() {// Outside (3rd item) executes when selected from mainMenu
  floorCounter = 3;//counter to use it in threeitemsMenu cases
  twoItemsMenu(outside,((sizeof(outside) / sizeof(String)) - 1),floorCounter);
}

void bedroom1(){ // first item from first floor
  floorCounter = 1;//counter to use it in threeItemsMenu cases
  threeItemsMenu(bedroomUtilities,((sizeof(bedroomUtilities) / sizeof(String)) - 1),floorCounter);
}

void bedroom2(){ // second item from first floor
  floorCounter = 1;//counter to use it in threeItemsMenu cases
  threeItemsMenu(bedroomUtilities,((sizeof(bedroomUtilities) / sizeof(String)) - 1),floorCounter);
  
}
void bathroom(){ // third item from first floor
  floorCounter = 1;//counter to use it in twoItemsMenu cases
  twoItemsMenu(basicUtilities,((sizeof(basicUtilities) / sizeof(String)) - 1),floorCounter);
  
}
void kitchen(){ // first item from ground floor
  floorCounter = 2;//counter to use it in twoItemsMenu cases
  twoItemsMenu(basicUtilities,((sizeof(basicUtilities) / sizeof(String)) - 1),floorCounter);
  
}
void hall(){  // second item from ground floor
  floorCounter = 2;//counter to use it in twoItemsMenu cases
  twoItemsMenu(basicUtilities,((sizeof(basicUtilities) / sizeof(String)) - 1),floorCounter);
  
}
void living(){  // third item from ground floor
  floorCounter = 2;//counter to use it in twoItemsMenu cases
  twoItemsMenu(basicUtilities,((sizeof(basicUtilities) / sizeof(String)) - 1),floorCounter);
  
}
void garageOutside(){ // first item from outside
  twoItemsMenu(garage,((sizeof(garage) / sizeof(String)) - 1),floorCounter);
}
void gardenOutside(){ // second item from outside
  twoItemsMenu(garden,((sizeof(garden) / sizeof(String)) - 1),floorCounter);
}

//When select button is clicked on an Utility it renders a new window to select on/off with left and right button
void lightLampBedroom(int arrayParameter){
  int going = 0;
  int button;
  int r;
  
  if(all[arrayParameter].endsWith("OFF"))
    r = 1;
  else
    r = 2;
  OnOffPagesDraw(bedroomUtilities,maxThreePages,r);
  drawCursor();
  
  while (going == 0) {
    readKey = lcd.readButtons();
    if (readKey < 6) {
      delay(50);
      readKey = lcd.readButtons();
    }
    button = readButtons(readKey);
    switch(button){
                  case 0:
                    break;
                  case 1:// select button
                    menuPage = 0;
                    cursorPos = 0;
                    button = 0;
                    Utilities = 0;// returning back to previous screen
                    OnOffPagesDraw(bedroomUtilities,maxThreePages,r);
                    drawCursor();
                    going = 1;
                    break;
                  case 4:// left button
                    
                    r = 1;
                    all[arrayParameter].replace("ON","OFF");// taking the value from data array and modifies it
                    OnOffPagesDraw(bedroomUtilities,maxThreePages,r);
                    
                    button = 0;
                    going = 1;
                    break;
                  case 5:// right button
                    
                    r = 2;
                    all[arrayParameter].replace("OFF","ON");// taking the value from data array and modifies it
                    OnOffPagesDraw(bedroomUtilities,maxThreePages,r);
                    
                    button = 0;
                    going = 1;
                    break;
              }  
      }
      
}

//When select button is clicked on an Utility it renders a new window to select on/off with left and right button but its for 2 utility pages
void lightTwoPages(int arrayParameter){
  int going = 0;
  int button;
  int r;  //On/Off counter
  if(all[arrayParameter].endsWith("OFF"))
    r = 1;
  else
    r = 2;
  if(roomsCounter == 10)
    OnOffTwoPagesDraw(garage,r);
  else if(roomsCounter == 11)
    OnOffTwoPagesDraw(garden,r);
  else
    OnOffTwoPagesDraw(basicUtilities,r);
  drawCursor();
  while (going == 0) {
    readKey = lcd.readButtons();
    if (readKey < 6) {
      delay(50);
      readKey = lcd.readButtons();
    }
    button = readButtons(readKey);
    switch(button){
                  case 0:
                    break;
                  case 1:// select button
                    menuPage = 0;
                    cursorPos = 0;
                    button = 0;
                    Utilities = 0;// returning back to previous screen
                    if(roomsCounter == 10)
                      OnOffTwoPagesDraw(garage,r);
                    else if(roomsCounter==11)
                      OnOffTwoPagesDraw(garden,r);
                    else
                      OnOffTwoPagesDraw(basicUtilities,r);
                    drawCursor();
                    going = 1;
                    break;
                  case 4:// left button
                    
                    r=1;
                    all[arrayParameter].replace("ON","OFF");// taking the value from data array and modifies it
                    if(roomsCounter==10)
                      OnOffTwoPagesDraw(garage,r);
                    else if(roomsCounter==11)
                      OnOffTwoPagesDraw(garden,r);
                    else
                      OnOffTwoPagesDraw(basicUtilities,r);
                    button = 0;
                    going = 1;
                    break;
                  case 5:// right button
                    
                    r=2;
                    all[arrayParameter].replace("OFF","ON");// taking the value from data array and modifies it
                    if(roomsCounter==10)
                      OnOffTwoPagesDraw(garage,r);
                    else if(roomsCounter==11)
                      OnOffTwoPagesDraw(garden,r);
                    else
                      OnOffTwoPagesDraw(basicUtilities,r);
                    button = 0;
                    going = 1;
                    break;
              }  
      }
      
}

//When select button is clicked on Heat it renders a new window to select levels with left and right button
void heatBedroom(int arrayParameter){
  int going = 0;
  int button;
  int r; // level counter 
  int j; // level counter which will be converted to String

  if(all[arrayParameter].substring(all[arrayParameter].length() - 1).toInt() == 0) // will get last digit and compares it to 0
    r = 0;
  if(r < 10)
    r=all[arrayParameter].substring(all[arrayParameter].length() - 1).toInt(); // r will get the digit which is last character of the String
  else if(r < 100)// will get last 2 digits
    r=all[arrayParameter].substring(all[arrayParameter].length() -2 ).toInt();
  if(roomsCounter > 5) // when roomsCounter>5 this func will be rendering only 2 pages
    heatTwoPageDraw(basicUtilities , all[arrayParameter].substring(all[arrayParameter].length() - 1)); 
  else // will render the arrows for 3 pages
    heatPageDraw(bedroomUtilities , maxThreePages , all[arrayParameter].substring(all[arrayParameter].length() - 1));
  drawCursor();
  while (going == 0) {
    readKey = lcd.readButtons();
    if (readKey < 6) {
      delay(50);
      readKey = lcd.readButtons();
    }
    button = readButtons(readKey);
    switch(button){
                  case 0:
                    break;
                  case 1: 
                    menuPage = 0;
                    cursorPos = 0;
                    button = 0;
                    
                    if(roomsCounter>5 )
                      heatTwoPageDraw(basicUtilities , all[arrayParameter].substring(all[arrayParameter].length() - 1));
                    else
                      heatPageDraw(bedroomUtilities , maxThreePages , all[arrayParameter].substring(all[arrayParameter].length() - 1));
                    Utilities = 0; // will go back to previous screen
                    drawCursor();
                    going = 1;
                    break;
                  case 4:// will decrement level of heat
                    
                    r--; 
                    j = r;
                    
                    all[arrayParameter].replace(all[arrayParameter].substring(all[arrayParameter].length() - 1) , String(j));
                    
                    if(roomsCounter > 5)
                      heatTwoPageDraw(basicUtilities , all[arrayParameter].substring(all[arrayParameter].length() - 1));
                    else
                      heatPageDraw(bedroomUtilities , maxThreePages , all[arrayParameter].substring(all[arrayParameter].length() - 1));
                      
                    button = 0;
                    going = 1;
                    break;
                  case 5:// will increment level of heat
                    
                    r++;
                    j = r;
                    
                    all[arrayParameter].replace(all[arrayParameter].substring(all[arrayParameter].length() - 1) , String(j));
                    
                    if(roomsCounter > 5 && roomsCounter < 11)
                      heatTwoPageDraw(basicUtilities , all[arrayParameter].substring(all[arrayParameter].length() - 1));
                    else
                      heatPageDraw(bedroomUtilities , maxThreePages , all[arrayParameter].substring(all[arrayParameter].length() - 1));
        
                    button = 0;
                    going = 1;
                    break;
              }  
      }
      
}


void serialPrintCreate(){ //I'm generating an array of all rooms floors and utilities for serial print and manipulating data
  int r = 0; // index for utilities
  int g = 0; // index for rooms
  int q = 0; // index for floors
  for(int j = 1 ; j <= 18 ; j++)
    {
       all[j].concat(mainMenu[q]+"/");
       
       if(all[j] == "FIRST/")
          all[j].concat(firstFloor[g]+"/");
       else if(all[j] == "GROUND/")
          all[j].concat(groundFloor[g]+"/");
       else if(all[j] == "OUTSIDE/")
          all[j].concat(outside[g]+"/");
          
       if(all[j] == "FIRST/BEDROOM1/" || all[j] == "FIRST/BEDROOM2/")
          all[j].concat(bedroomUtilities[r]+"/");
       else if(all[j] == "OUTSIDE/GARAGE/")
          all[j].concat(garage[r]+"/");
       else if(all[j] == "OUTSIDE/GARDEN/")
          all[j].concat(garden[r]+"/");
       else
          all[j].concat(basicUtilities[r]+"/");
       
       all[j].concat("Main/");
       if(all[j].endsWith("HEAT/Main/"))
          all[j].concat("LEVEL:0");
       else
          all[j].concat("OFF");
             
       r++;
      
       if(r == 3 || (r == 2 && g == 2) || (r == 2 && q == 1) || (r == 2 && q == 2))
       {
          g++;
          r = 0;
       }
       if(g == 3)
       {
          q++;
          g = 0;
       }
       
    }
     
  
}

void serialPrint(){ // this function is used for Query to print curent state of all utilities

  for (int j = 1; j <= 18 ; j++)
        Serial.println(all[j]);
}
