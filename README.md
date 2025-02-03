# PreEmploymentTestTasks
Pre-Employment test program tasks that have been given to me with my solutions to them.


**1. COLORED_SQUARE**

Program has one window that shows 3x3 square grid and some geolocation and meteorological data (they are not connected, it's just for testing candidate's programming skills). Squares are colored in 9 different colors that are randomly assigned when you start the program, also there's a 10th color that is vacant. Clicking on any square will swap it's color with the vacant color.

My solution functionality:
- CMAKE build that can be built in many different OS
- Frameworks used are very lightweight (no Qt), also they are all included in the repository or are attached to build via CMAKE's FetchContent
- Program sends web-requests at runtime, which return current location and meteo-data, based on your machine's IP adress
- If web-requests fail, program shows current system time
- Showed data is synchronised based on the system time first and then after 1 minute it is synchronised on web-data (for adequate UX)
- Web-services list can be easily changed in DefineConstants.h along with the other constants mainly relating to UI colors
- If you chose to change web-services list, you would need to scale the requests code block accordingly, code is written in easily-scalable way

List of used frameworks:
- GLFW (window creation handler)
- GLAD (OpenGL wrapper)
- DearImGui (GUI)
- CPR (C++ web-requests)
- nlohmann (JSON parser lib)

!!Please unpack dependencies archive before compiling. I packed it because it has too many files to be commited at once.

![image](https://github.com/user-attachments/assets/0f1b08ce-07ef-4c65-aef9-2e24cabdb10f)
