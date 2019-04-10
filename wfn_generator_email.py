#!/usr/bin/python
# Luca Sevà
# Extract AIMPAC data from output file of GAMESS
#This script is designed to extract AIMPAC data from an output file (.dat) of the GAMESS program.
#If valid data is found these are written to a .wfn file (wavefunction file). After the newly generated file have been saved the script will automatically send it to a designated email address specified into the script.
#NOTE: the script will check for the dimension of the file to ensure that can be attached to the email. The size limit is of 25MB for Gmail.
# Last updated : 4-10-2019
import sys
import os
import os.path
import datetime
import smtplib, email, ssl

from subprocess import call
from shutil import copyfile
from os.path import basename
from email import encoders
from email.mime.base import MIMEBase
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText

#Information for email
subject = "WFN file is ready!"
sender_email = "sent_from@gmail.com"
receiver_email = "receiver@gmail.com"
password = 'type here password'

# Create a multipart message and set headers
message = MIMEMultipart()
message["From"] = sender_email
message["To"] = receiver_email
message["Subject"] = subject
message["Bcc"] = receiver_email  # Recommended for mass emails


# HERE GIVE THE FULL PATH TO GAMESS FOLDER:
path_to_gamess_dat_files = "C:\\gamess-64\\restart\\" #DEFAULT

# HERE GIVE THE VERSION OF GAMESS YOU HAVE:
version = "2018-R1-pgi-mkl"

#Check arguments
if len(sys.argv) <= 1:
	input_file = raw_input("Enter path to input: ")
else:
	input_file = sys.argv[1] 

#Separating name with extensions	
name = input_file.split(".dat")[0]
output_name = name+".wfn"

#Check if file exists
if os.path.isfile(path_to_gamess_dat_files+input_file) is False:
  print("No such file found!")
  sys.exit()
else:
  print("Opening C:\\gamess-64\\restart\\" + input_file)
  in_file = open(path_to_gamess_dat_files+input_file, "rt")

#Preliminary search for AIMPAC data
doIHaveToCopyTheLine=False
for line in in_file.readlines():
  if 'GAUSSIAN' in line:
    doIHaveToCopyTheLine=True
in_file.close()

if doIHaveToCopyTheLine is False:
  print("No AIMPAC data found in file!")
  sys.exit()
else:
  in_file = open(path_to_gamess_dat_files+input_file, "rt")
  out_file = open(output_name, 'a')
  out_file.write('Title\n')
  doIHaveToCopyTheLine=False
  for line in in_file.readlines():
    if 'GAUSSIAN' in line:
      doIHaveToCopyTheLine=True
    if doIHaveToCopyTheLine:
      out_file.write(line)
      print(line)
in_file.close()              
out_file.close()
#Get output file size in MB
statinfo = os.path.getsize(output_name)/1048576
print("Output file size (MB): ",statinfo) 
#Check for attachment dimension
if statinfo < 25:
  #Now we have the name of the file will be outputing
  body = "WFN file for " + name + " has been generated!\nFile has been created @: " + datetime.datetime.now().strftime("%I:%M%p on %B %d, %Y") +"\nCheck it out!"
  # Add body to email
  message.attach(MIMEText(body, "plain"))
  # Open WFN file in binary mode
  with open(output_name, "rb") as attachment:
      # Add file as application/octet-stream
      # Email client can usually download this automatically as attachment
      part = MIMEBase("application", "octet-stream")
      part.set_payload(attachment.read())
  # Encode file in ASCII characters to send by email    
  encoders.encode_base64(part)
  # Add header as key/value pair to attachment part
  part.add_header(
      "Content-Disposition",
      f"attachment; filename= {output_name}",
  )
  # Add attachment to message and convert message to string
  message.attach(part)
else:
  body = "WFN file for " + name + " has been generated!\nFile has been created @: " + datetime.datetime.now().strftime("%I:%M%p on %B %d, %Y\n") +"Due to the size limitations of the attachments it was not possible to attach the output file."
  message.attach(MIMEText(body, "plain"))
text = message.as_string()
context = ssl.create_default_context()
try:
  context = ssl.create_default_context()
  with smtplib.SMTP_SSL("smtp.gmail.com", 465, context=context) as server:
    server.login(sender_email, password)
    server.sendmail(sender_email, receiver_email, text)
  server.close()
  print("Email sent!")
except:  
  print("Something went wrong...and it's you")
print( ">> COMPLETE.")
