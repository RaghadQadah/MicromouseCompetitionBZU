/************************************************************************************************************************************************************************                                              
 * - Author               : BELKHIR Mohamad              *                                               
 * - Profession           : (Developer) MEGA DAS owner   *                                                                                              
 * - Main purpose         : Industrial Application       *                                                                                 
 * - Copyright (c) holder : All rights reserved          *
 * - License              : BSD 2-Clause License         * 
 * - Date                 : 07/07/2019                   *
 * ***********************************************************************************************************************************************************************/
 
 /*********************************** NOTE **************************************/
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// * Redistributions of source code must retain the above copyright notice, this
//  list of conditions and the following disclaimer.

// * Redistributions in binary form must reproduce the above copyright notice,
//  this list of conditions and the following disclaimer in the documentation
//  and/or other materials provided with the distribution.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED

/*
┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈╱▔▔▔▔▔▔╲┈╭━━━━━━━━━╮┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈
┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈▕┈╭━╮╭━╮┈▏┃ L298N DRIVER… ┃┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈
┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈▕┈┃╭╯╰╮┃┈▏╰┳━━━━━━━━╯┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈
┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈▕┈╰╯╭╮╰╯┈▏┈┃┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈
┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈▕┈┈┈┃┃┈┈┈▏━╯┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈
┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈▕┈┈┈╰╯┈┈┈▏┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈
┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈▕╱╲╱╲╱╲╱╲▏┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈
 */
const int IN1=8;
const int IN2=7;
const int EN1=9;
const int Potentiometer=A0;
int MotorSpeed=0;
char data;
String Direction="";
void setup() 
{
  Serial.begin(9600);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(EN1,OUTPUT);
  Serial.println("******************************************************************************************************");
  Serial.println("                              L298N DRIVER SPEED AND DIRECTION CONTROL                                ");
  Serial.println("******************************************************************************************************");
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
}

void loop() 
{
  while(Serial.available())
  {
    delay(10);
    data=Serial.read();
    Direction+=data;
  }
  //-------------------------------------------- SPEED CONTROLL -----------------------------------------------------------------------
  MotorSpeed=map(analogRead(Potentiometer), 0, 1023, 0, 255); // Read the speed from the potentiometer analog value from 0 to 5V ==> 0 to 255
  analogWrite(EN1, MotorSpeed);                               // Write the speed converted speed to PWM output 

  //-------------------------------------------- DIRECTION CONTROLL -----------------------------------------------------------------------
  if(Direction=="left")                                       // Left direction command
  {
    digitalWrite(IN1,HIGH);
    digitalWrite(IN2,LOW);
    Serial.println("Motor run left direction");
  }
  if(Direction=="right")                                      // Right direction command
  {
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,HIGH);
    Serial.println("Motor run right direction");
  }
  Direction="";
}
