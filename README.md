# Prosjektoppgave
Prosjektoppgave datakomm

Requirements for the project
Mandatory part
The goal of the project is to build an Internet-of-Things (IoT) system which consists of the following
elements:
1. A physical unit interacting with the world. Let us call this part the robot (in the document
“Prosjektoppgaven” is this device referred to as “ESP32”). Interaction can be in any of the two
directions:
a. Reading some phenomena by using sensors (temperature, humidity, distance, light,
sound, CO2 level, …)
b. Acting – moving something, making sounds, blinking lights, honking, telling stories in
robot voice, …
2. A remote user interface allowing to interact with the robot. Interaction can again mean any of
the two directions:
a. Visualizing the measured sensor data: charts, tables, alerts, …
b. Sending commands to the robot: driving, moving, sounds, lights, open/close doors,
windows, …
3. Communication between the robot and interface over the Internet.
Hand-ins
To get a grade, you will hand in the following in the Inspera system:
1. Source code of all components in your project (as a ZIP file).
2. A report describing what you have done.
3. A 5-10 min video showing the product and explaining it (at a high level).
Source code and instructions
The source code hand-in should include all the source code you have developed in the group, the
complete solution (including all the code templates you re-use). In other words: the examiner should be
able to take your code and run the whole system.
Report content
The report should contain the following sections:
1. General description of the product – what is the system supposed to do? What application does
it have?
2. Which technologies have you chosen? Both hardware and software.
3. General system architecture, preferably a diagram showing the components and a description
for each of them.
4. System setup instructions. The instructions (can be a .txt file, or a README file format) should
describe enough information for a technical person to be able to set up your system. Part of
engineering profession is to document the work that has been done (and how to repeat it).
5. Reflection on the chosen technologies. What is good with the protocols and solution you have
chosen? Are there any drawbacks? Even if you choose the suggested technologies and
components (Raspberry Pi, ESP32, Websockets, etc), you should reflect on why this is a good 
solution? It is not enough to say “we chose it because it was the default option and the teachers
should know what they suggest”. What is good with ESP32? What is good with Websockets?
When you choose any component, you should read up on it on the web or other sources – what
are the advantages and drawbacks? Are these satisfactory for your solution? Are there better
alternatives?
6. Reflection on the connection between theory and project. Which application-layer protocols do
you use in the project? Which transport-layer protocols? How is global addressing ensured?
How is the communication secured?
Deadlines
There is only a single deadline for project delivery, see date for Mappevurdering in the course
description.
Technologies
We suggest a set of technologies to use. However, you can replace the technology for any of the system
components with anything else, as long as you can still satisfy the main requirements – an IoT system
with remote user interface.
The default technology suggestion:
• ESP32 node as the robot. Connect sensors to it, using a breadboard.
• A Raspberry pie to run other software parts of the system, see below.
• A web page for user interface. For that to be possible, you will need to host the web-site
somewhere. If you are familiar with web development, host the site wherever you prefer. If not,
it is suggested to run a web server on a Raspberry Pi, we will provide instructions on how to do
that.
• Communication between the web and robot using WebSockets. For that to work, you will need
a web-socket server. Both the web-page and the robot become clients in this case. We provide
template on how to run a websocket server on a Raspberry Pi.
Strictly speaking, the raspberry pie is not necessary – you can run it on any computer, including a server
in the cloud. Raspberry Pie is a platform commonly used for simple IoT projects – it is a low-cost box that
can function well as a server for hosting different application-layer services.
Grading
Considering that there are no presentations of the project, the grading is done solely based on the
report. There is no specific formula on how the grade is chosen. The following things will be considered
in grading:
1. A working system is built consisting of all the mandatory components.
2. The required sections of the report are present and describe the necessary things in a good way.
3. Appropriate engineering report structure and style is maintained.
4. Reflection parts show that students have understood the used protocols and connection
between theory and project.
