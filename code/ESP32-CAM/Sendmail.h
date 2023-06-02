#ifndef SENDMAIL_H
#define SENDMAIL_H
#include "ESP32_MailClient.h"
#include <Arduino.h>
#include "esp_camera.h"
#include <FS.h>
#include <WiFi.h>
#include "SPIFFS.h"
#include "esp_timer.h"
#include "img_converters.h"

#define emailSenderAccount    "notifysystem@hsute.onmicrosoft.com"    
#define emailSenderPassword   "Cuaanninhso123"
#define emailRecipient        "sonvu20489@gmail.com"
#define smtpServer            "smtp.office365.com"
#define smtpServerPort        587
#define emailSubject          "[Warnning] illegal access"


SMTPData smtpData;
void sendCallback(SendStatus msg) {
  // Print the current status
  Serial.println(msg.info());
  // Do something when complete
  if (msg.success()) {
    Serial.println("----------------");
  }
}
void sendmail(void)
{
  if(WiFi.status()!=WL_CONNECTED)
  {
    return ;
  }
//  smtpData.setLogin(smtpServer, smtpServerPort, emailSenderAccount, emailSenderPassword);
//  // Set the sender name and Email
//  smtpData.setSender("Notify system center", emailSenderAccount);
//  // Set Email priority or importance High, Normal, Low or 1 to 5 (1 is highest)
//  smtpData.setPriority("High");
//  // Set the subject
//  smtpData.setSubject(emailSubject);
//  // Set the message with HTML format
//  smtpData.setMessage("<div style=\"color:yellow;\"><h1>Warninggggg!</h1><p>- Warning: Illegal access detected! </p><br><br><p>- Image taken--</p></div>", true);
//  // Set the email message in text format (raw)
//  //smtpData.setMessage("Hello World! - Sent from NodeWifi32 board", false);
//  // Add recipients, you can add more than one recipient
//  smtpData.addRecipient(emailRecipient);
//  //smtpData.addRecipient("YOUR_OTHER_RECIPIENT_EMAIL_ADDRESS@EXAMPLE.com");
//  smtpData.setFileStorageType(MailClientStorageType::SPIFFS);
//  smtpData.addAttachFile("/photo.jpg");
//  smtpData.setSendCallback(sendCallback);
//  //Start sending Email, can be set callback function to track the status
//  if (!MailClient.sendMail(smtpData))
//    Serial.println("Error sending Email, " + MailClient.smtpErrorReason());
//  //Clear all data from Email object to free memory
//  smtpData.empty();
  smtpData.setLogin(smtpServer, smtpServerPort, emailSenderAccount, emailSenderPassword);
  // Set the sender name and Email
  smtpData.setSender("Notify system center", emailSenderAccount);
  // Set Email priority or importance High, Normal, Low or 1 to 5 (1 is highest)
  smtpData.setPriority("High");
  // Set the subject
  smtpData.setSubject(emailSubject);
  // Set the message with HTML format
  smtpData.setMessage("<div style=\"color:#2f4468;\"><h1>Hello World!</h1><p>- Sent from NodeWifi32 board</p></div>", true);
  // Set the email message in text format (raw)
  //smtpData.setMessage("Hello World! - Sent from NodeWifi32 board", false);
  // Add recipients, you can add more than one recipient
  smtpData.addRecipient(emailRecipient);
  //smtpData.addRecipient("YOUR_OTHER_RECIPIENT_EMAIL_ADDRESS@EXAMPLE.com");
  smtpData.setSendCallback(sendCallback);
  //Start sending Email, can be set callback function to track the status
  if (!MailClient.sendMail(smtpData))
    Serial.println("Error sending Email, " + MailClient.smtpErrorReason());
  //Clear all data from Email object to free memory
  smtpData.empty();
}
#endif
