#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
SoftwareSerial rfid(A0, A1);

#define buzzer 10
#define check_bill 0  // Changed from 'o' (letter o) to 0 (digital pin 0). Adjust as per your wiring.
#define green_led A4
#define red_led A5

String master_card_id = "";
String item_1_card_id = "";
String item_2_card_id = "";
String item_3_card_id = "";
String card_id_2 = "";

int item_1_cost = 100;
int item_2_cost = 200;
int item_3_cost = 300;
int total_cost = 0;

bool item_1_added = false;
bool item_2_added = false;
bool item_3_added = false;
bool card_detected = false;
bool bill_status = false;

void setup() {
  rfid.begin(9600);

  pinMode(buzzer, OUTPUT);
  pinMode(green_led, OUTPUT);
  pinMode(red_led, OUTPUT);
  pinMode(check_bill, INPUT);

  digitalWrite(buzzer, LOW);
  digitalWrite(green_led, HIGH);
  digitalWrite(red_led, HIGH);

  lcd.begin(20, 4);
  lcd.setCursor(1, 0);
  lcd.print("Auto Billing Mall");  // Fixed typo: was correct
  lcd.setCursor(3, 1);
  lcd.print("Shopping Cart");      // Fixed typo: 'Icd' → 'lcd'
  lcd.setCursor(5, 2);
  lcd.print("With Arduino");

  delay(5000);
  lcd.clear();

  lcd.print("SWIPE-REGISTER THE");
  lcd.setCursor(5, 1);
  lcd.print("MASTER CARD");

  master_card_id = register_rfid_cards(); // Read master card here

  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Master card ID is:");
  lcd.setCursor(0, 2);
  lcd.print(master_card_id);
  delay(1000);
  lcd.clear();

  for (int i = 1; i <= 3; i++) { // i <= 3 to include 3rd item
    lcd.clear();
    lcd.print("SWIPE THE CARD ");
    lcd.setCursor(0, 1);
    lcd.print("FOR Item no. ");
    lcd.print(i);

    String card_id = register_rfid_cards();
    lcd.clear();

    if (i == 1) {
      item_1_card_id = card_id;
    } else if (i == 2) {
      item_2_card_id = card_id;
    } else if (i == 3) {
      item_3_card_id = card_id;
    }

    lcd.setCursor(0, 1);
    lcd.print("Item no.: ");
    lcd.print(i);
    lcd.setCursor(0, 2);
    lcd.print(card_id);
    delay(1000);
  }
}

void loop() {
  lcd.clear();
  lcd.print("Swipe The Product");
  lcd.setCursor(0, 3);
  lcd.print("COST: ");
  lcd.setCursor(9, 3);
  lcd.print(total_cost);

  delay(100);

  check_cart_status();

  bill_status = digitalRead(check_bill);

  if (bill_status == LOW) { // changed 0 to LOW for clarity
    check_bill_status();
  }
}

void check_bill_status() {
  lcd.clear();

  if (item_1_added) {
    lcd.setCursor(0, 0);
    lcd.print("Item 1 = ");
    lcd.print(item_1_cost);
  }

  if (item_2_added) {
    if (item_1_added) {
      lcd.setCursor(0, 1);
    } else {
      lcd.setCursor(0, 0);
    }
    lcd.print("Item 2 = ");
    lcd.print(item_2_cost);
  }

  if (item_3_added) {
    if (item_1_added && item_2_added) {
      lcd.setCursor(0, 2);
    } else if (!item_1_added && !item_2_added) {
      lcd.setCursor(0, 0);
    } else {
      lcd.setCursor(0, 1);
    }
    lcd.print("Item 3 = ");
    lcd.print(item_3_cost);
  }

  if (!item_1_added && !item_2_added && !item_3_added) {
    lcd.print("Cart Is Empty");
  }

  lcd.setCursor(0, 3);
  lcd.print("Total Bill = ");
  lcd.print(total_cost);
  delay(2000);
}

void check_cart_status() {
  card_id_2 = read_rfid_card();

  if (card_id_2 == item_1_card_id) {
    if (!item_1_added) {
      green_on_red_off();
      delay(300);
      green_off_red_off();
      total_cost += item_1_cost;
      item_1_added = true;
      lcd.clear();
      lcd.print("Item 1");
      lcd.setCursor(0, 1);
      lcd.print("Added To Cart");
      lcd.setCursor(0, 2);
      lcd.print("Price = ");
      lcd.print(item_1_cost);
      delay(2000);
    } else {
      green_off_red_on();
      delay(300);
      green_off_red_off();
      total_cost -= item_1_cost;
      item_1_added = false;
      lcd.clear();
      lcd.print("Item 1");
      lcd.setCursor(0, 1);
      lcd.print("Removed From Cart");
      lcd.setCursor(0, 2);
      lcd.print("Price = ");
      lcd.print(item_1_cost);
      delay(2000);
    }
  } 
  else if (card_id_2 == item_2_card_id) {
    if (!item_2_added) {
      green_on_red_off();
      delay(300);
      green_off_red_off();
      total_cost += item_2_cost;
      item_2_added = true;
      lcd.clear();
      lcd.print("Item 2");
      lcd.setCursor(0, 1);
      lcd.print("Added To Cart");
      lcd.setCursor(0, 2);
      lcd.print("Price = ");
      lcd.print(item_2_cost);
      delay(2000);
    } else {
      green_off_red_on();
      delay(300);
      green_off_red_off();
      total_cost -= item_2_cost;
      item_2_added = false;
      lcd.clear();
      lcd.print("Item 2");
      lcd.setCursor(0, 1);
      lcd.print("Removed From Cart");
      lcd.setCursor(0, 2);
      lcd.print("Price = ");
      lcd.print(item_2_cost);
      delay(2000);
    }
  } 
  else if (card_id_2 == item_3_card_id) {
    if (!item_3_added) {
      green_on_red_off();
      delay(300);
      green_off_red_off();
      total_cost += item_3_cost;
      item_3_added = true;
      lcd.clear();
      lcd.print("Item 3");
      lcd.setCursor(0, 1);
      lcd.print("Added To Cart");
      lcd.setCursor(0, 2);
      lcd.print("Price = ");
      lcd.print(item_3_cost);
      delay(2000);
    } else {
      green_off_red_on();
      delay(300);
      green_off_red_off();
      total_cost -= item_3_cost;
      item_3_added = false;
      lcd.clear();
      lcd.print("Item 3");
      lcd.setCursor(0, 1);
      lcd.print("Removed From Cart");
      lcd.setCursor(0, 2);
      lcd.print("Price = ");
      lcd.print(item_3_cost);
      delay(2000);
    }
  } 
  else if (card_id_2 == master_card_id) {
    check_bill_status();
    lcd.clear();
    lcd.print("Purchase Verified");
    lcd.setCursor(0, 2);
    lcd.print("At Billing Counter");
    lcd.setCursor(0, 3);
    lcd.print("Thank You!");
    while (1);  // Halt here
  }

  card_id_2 = "";
}

String read_rfid_card() {
  String rfid_tag = "";
  while (rfid.available() > 0) {
    rfid_tag += (char)rfid.read();
    delay(2);
    card_detected = true;
  }
  if (card_detected) {
    digitalWrite(buzzer, HIGH);
    delay(200);
    digitalWrite(buzzer, LOW);
    rfid_tag.trim();
    card_detected = false;
    if (rfid_tag == master_card_id) {
      lcd.clear();
      lcd.print("Master Card");
      lcd.setCursor(0, 1);
      lcd.print("detected");
      digitalWrite(buzzer, HIGH);
      delay(3000);
      digitalWrite(buzzer, LOW);
    }
    return rfid_tag;
  }
  return "";
}

String register_rfid_cards() {
  String rfid_tag = "";
  while (rfid.available() == 0);  // Wait for data
  while (rfid.available() > 0) {
    rfid_tag += (char)rfid.read();
    delay(2);
  }
  digitalWrite(buzzer, HIGH);
  delay(200);
  digitalWrite(buzzer, LOW);
  rfid_tag.trim();
  return rfid_tag;
}

void green_on_red_off() {
  digitalWrite(green_led, LOW);
  digitalWrite(red_led, HIGH);
}

void green_off_red_on() {
  digitalWrite(green_led, HIGH);
  digitalWrite(red_led, LOW);
}

void green_off_red_off() {
  digitalWrite(green_led, HIGH);
  digitalWrite(red_led, HIGH);
}
