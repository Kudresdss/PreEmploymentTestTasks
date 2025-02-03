#include "ColoredSquare.h"

//////////////////////////////////////////////////////////////////////
// Конструктор и деструктор

ColoredSquare::ColoredSquare()
{	
	setupView();
}

ColoredSquare::~ColoredSquare()
{
	std::cout << "\n\n" << "Last RequestLog:" << "\n" << lastRequestLog << std::endl;

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	if (window)
		glfwDestroyWindow(window);

	glfwTerminate();
}

//////////////////////////////////////////////////////////////////////
// Инициализация данных GUI

void ColoredSquare::setupView()
{
	// инициализация GLFW //

	glfwSetErrorCallback(glfw_error_callback);

	if (glfwInit() == GLFW_FALSE)
		throw std::runtime_error("Failed to initialize GLFW");

	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	window = glfwCreateWindow(1366, 768, "ColoredSquare", NULL, NULL);
	if (window == GLFW_FALSE) {
		throw std::runtime_error("Failed to create a window");
	}

	glfwSetKeyCallback(window, key_callback);
	glfwMakeContextCurrent(window);

	// инициализация GLAD //

	if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == false)
		throw std::runtime_error("Failed to initialize GLAD");

	// инициализация ImGui //

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// стиль GUI //

	ImGuiStyle* style = &ImGui::GetStyle();
	style->Colors[ImGuiCol_Text] = createImGuiColorFromColorRGB(STYLE_TEXT_COLOR_RGB_LIGHT_GREY);
}

//////////////////////////////////////////////////////////////////////
// Основная логика программы

void ColoredSquare::run()
{
	fillColorVectors();
	shuffleColorParsedFullDataVector();

	startView();
}

void ColoredSquare::startView()
{
	GUI_Data.clickOnSquaresCounter = 0;

	std::chrono::minutes oneMinute = std::chrono::minutes(1);
	std::chrono::time_point systemTimeStart = system_clock::now();
	std::chrono::time_point systemTimeMinuteLater = std::chrono::floor<std::chrono::minutes>(systemTimeStart + oneMinute);

	ImVec4 clearColor = createImGuiColorFromColorRGB(BACKGROUND_COLOR_RGB_DARK_GREEN);
	int32_t displayWidth{}, displayHeight{};

	auto f_requestDataFromWebServices = [this]() -> void
	{
			requestCount++;
			std::string requestNotify = "\nWeb-requests call: " + std::to_string(requestCount);
			std::cout << requestLogDelimiter << requestNotify << std::endl;

			requestGeolocationData();

			if (hasReturnedValidResponseGeolocation == true) {
				requestMeteoData();
				requestTimeData();
			}
	};

	std::cout << "RequestLog:" << std::endl;

	f_requestDataFromWebServices();
	requestCheckPoint = steady_clock::now();

	while (glfwWindowShouldClose(window) == GLFW_FALSE)
	{
		glfwPollEvents();

		if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0)
		{
			ImGui_ImplGlfw_Sleep(10);
			continue;
		}

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (isRequestTimeSynchronized == true)
		{
			std::chrono::duration runLoopDurationFromPreviousCheckpoint = steady_clock::now() - requestCheckPoint;
			if (runLoopDurationFromPreviousCheckpoint >= oneMinute)
			{
				f_requestDataFromWebServices();
				requestCheckPoint = steady_clock::now();
			}
		}
		else
		{
			if (system_clock::now() >= systemTimeMinuteLater)
			{
				f_requestDataFromWebServices();
				requestCheckPoint = steady_clock::now();

				isRequestTimeSynchronized = true;
			}
		}

		// вызывается свой GUI
		showGUI();

		ImGui::Render();
		
		if (glfwSetWindowSizeCallback(window, glfw_window_size_callback))
		{
			int width, height;
			glfwGetWindowSize(window, &width, &height);

			if (width < 1366)
				width = 1366;
			if (height < 768)
				height = 768;

			glfwSetWindowSize(window, width, height);
		}

		glfwGetFramebufferSize(window, &displayWidth, &displayHeight);

		glViewport(0, 0, displayWidth, displayHeight);
		glClearColor(clearColor.x * clearColor.w, clearColor.y * clearColor.w, clearColor.z * clearColor.w, clearColor.w);
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}
}

//////////////////////////////////////////////////////////////////////
// Разметка GUI

void ColoredSquare::showGUI()
{
	IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing Dear ImGui context. Refer to examples app!");
	IMGUI_CHECKVERSION();

	ImGuiWindowFlags windowFlags = 0;
	windowFlags |= ImGuiWindowFlags_NoTitleBar;
	windowFlags |= ImGuiWindowFlags_NoScrollbar;
	windowFlags |= ImGuiWindowFlags_NoMove;
	windowFlags |= ImGuiWindowFlags_NoResize;
	windowFlags |= ImGuiWindowFlags_NoBackground;
	windowFlags |= ImGuiWindowFlags_NoSavedSettings;

	const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 200, main_viewport->WorkPos.y + 50));
	ImGui::SetNextWindowSize(ImVec2(1280, 640));

	if (!ImGui::Begin("ColoredSquare", NULL, windowFlags))
	{
		ImGui::End();
		return;
	}

	ImGui::PushItemWidth(ImGui::GetFontSize() * -12);
	ImGui::SetWindowFontScale(1.5f);
	int32_t buttonSizeInPixels = 150;

	std::string instructionStr = "Press to swap square's color with vacant color";
	ImGui::SetCursorPos(ImVec2(2, 8));
	ImGui::Text(instructionStr.c_str());

	ImGui::Spacing();

	auto colorsIterator = GUI_Data.colorParsedFullDataVector.begin();
	auto colorsIteratorVacantColor = GUI_Data.colorParsedFullDataVector.end();
	colorsIteratorVacantColor--;

	for (uint32_t i = 0; i < 9; i++)
	{
		if (i != 0 && i != 3 && i != 6)
			ImGui::SameLine();
		else
			ImGui::Spacing();

		ImGui::PushID(i);

		std::string currentButtonColorName = (*colorsIterator).first;
		ImVec4 currentButtonColor = (*colorsIterator).second;

		if (compareColors(currentButtonColor, createImGuiColorFromColorRGB(COLOR_RGB_BLACK))	||
			compareColors(currentButtonColor, createImGuiColorFromColorRGB(COLOR_RGB_BLUE))		||
			compareColors(currentButtonColor, createImGuiColorFromColorRGB(COLOR_RGB_RED))		||
			compareColors(currentButtonColor, createImGuiColorFromColorRGB(COLOR_RGB_BROWN))	)
		{
			ImGui::PushStyleColor(ImGuiCol_Text, createImGuiColorFromColorRGB(STYLE_TEXT_COLOR_RGB_FULL_WHITE));
		}
		else
			ImGui::PushStyleColor(ImGuiCol_Text, createImGuiColorFromColorRGB(STYLE_TEXT_COLOR_RGB_FULL_BLACK));

		ImGui::PushStyleColor(ImGuiCol_Button, currentButtonColor);

		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(
			currentButtonColor.x - 0.05f,
			currentButtonColor.y - 0.05,
			currentButtonColor.z - 0.05,
			currentButtonColor.w)
		);

		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(
			currentButtonColor.x - 0.1f,
			currentButtonColor.y - 0.1f,
			currentButtonColor.z - 0.1f,
			currentButtonColor.w)
		);
		
		if (ImGui::Button(currentButtonColorName.c_str(), ImVec2(buttonSizeInPixels, buttonSizeInPixels))) {
			std::swap(*colorsIterator, *colorsIteratorVacantColor);
			GUI_Data.clickOnSquaresCounter++;
		}

		ImGui::PopStyleColor(4);
		ImGui::PopID();

		colorsIterator++;
	}

	std::string vacantColorStr = "Vacant color: ";
	ImVec4 vacantColorRGB = (*colorsIteratorVacantColor).second;

	ImGui::SetCursorPos(ImVec2(buttonSizeInPixels, buttonSizeInPixels * 3 + 95));
	ImGui::Text(vacantColorStr.c_str());

	ImGui::SetCursorPos(ImVec2(buttonSizeInPixels * 2, buttonSizeInPixels * 3 + 82));

	ImGui::PushID(10);
	ImGui::PushStyleColor(ImGuiCol_Button, vacantColorRGB);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, vacantColorRGB);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, vacantColorRGB);
	
	ImGui::Button("", ImVec2(buttonSizeInPixels / 3, buttonSizeInPixels / 3));
	ImGui::PopStyleColor(3);
	ImGui::PopID();

	std::string clickOnSquaresCountStr = "Colors swapped: " + std::to_string(GUI_Data.clickOnSquaresCounter);

	ImGui::SetCursorPos(ImVec2(buttonSizeInPixels, buttonSizeInPixels * 3 + 150));
	ImGui::Text(clickOnSquaresCountStr.c_str());

	std::string requestedTextData{};
	if (hasReturnedValidResponseGeolocation && hasReturnedValidResponseMeteo && hasReturnedValidResponseTime) {
		requestedTextData =
			"Ambient air temperature: " + temperature + " (In location: " + city + ")\n" +
			"Time: " + time + "\n" +
			"Date: " + date;
	}
	
	if (hasReturnedValidResponseGeolocation && !hasReturnedValidResponseMeteo && !hasReturnedValidResponseTime){
		std::time_t currentSystemTime = system_clock::to_time_t(system_clock::now());
		std::string currentSystemTimeStr = ctime(&currentSystemTime);
		
		requestedTextData =
			"Ambient air temperature, time and date: Failed to connect to\nmeteorological and global time web-services\nLocation: " + city +
			"\nSystem time: " + currentSystemTimeStr;
	}
	else if (hasReturnedValidResponseGeolocation && !hasReturnedValidResponseMeteo && hasReturnedValidResponseTime) {
		requestedTextData =
			"Ambient air temperature: Failed to connect to\nmeteorological web-services\nLocation: " + city + "\n" +
			"Time: " + time + "\n"
			"Date: " + date;
	}
	else if (hasReturnedValidResponseGeolocation && hasReturnedValidResponseMeteo && !hasReturnedValidResponseTime) {
		std::time_t currentSystemTime = system_clock::to_time_t(system_clock::now());
		std::string currentSystemTimeStr = ctime(&currentSystemTime);

		requestedTextData =
			"Time and date: Failed to connect to\nglobal time web-services\nAmbient air temperature: " + temperature + " (In location: " + city +
			")\nSystem time: " + currentSystemTimeStr;
	}
	else if (!hasReturnedValidResponseGeolocation) {
		std::time_t currentSystemTime = system_clock::to_time_t(system_clock::now());
		std::string currentSystemTimeStr = ctime(&currentSystemTime);

		requestedTextData = "Failed to connect to geolocation web-services:\ncan't fetch any online data :(\nSystem time: " + currentSystemTimeStr;
	}
	

	ImGui::SetCursorPos(ImVec2(buttonSizeInPixels * 3 + 50, 40));
	ImGui::Text(requestedTextData.c_str());

	ImGui::End();
}


//////////////////////////////////////////////////////////////////////
// Вспомогательные методы для работы с вектором цветов квадратов и их названий

void ColoredSquare::fillColorVectors()
{
	GUI_Data.colorFullDataVector.push_back({ COLOR_NAME_WHITE,		COLOR_RGB_WHITE });
	GUI_Data.colorFullDataVector.push_back({ COLOR_NAME_BLACK,		COLOR_RGB_BLACK });
	GUI_Data.colorFullDataVector.push_back({ COLOR_NAME_BROWN,		COLOR_RGB_BROWN });
	GUI_Data.colorFullDataVector.push_back({ COLOR_NAME_BLUE,		COLOR_RGB_BLUE });
	GUI_Data.colorFullDataVector.push_back({ COLOR_NAME_LIGHT_BLUE,	COLOR_RGB_LIGHT_BLUE });
	GUI_Data.colorFullDataVector.push_back({ COLOR_NAME_RED,		COLOR_RGB_RED });
	GUI_Data.colorFullDataVector.push_back({ COLOR_NAME_YELLOW,		COLOR_RGB_YELLOW });
	GUI_Data.colorFullDataVector.push_back({ COLOR_NAME_GREEN,		COLOR_RGB_GREEN });
	GUI_Data.colorFullDataVector.push_back({ COLOR_NAME_GREY,		COLOR_RGB_GREY });
	GUI_Data.colorFullDataVector.push_back({ COLOR_NAME_PINK,		COLOR_RGB_PINK });

	auto colorFullDataIterator = GUI_Data.colorFullDataVector.begin();
	for (uint32_t i = 0; i < 10; i++)
	{
		auto [x, y, z, w] = breakConstantColorCode( (*colorFullDataIterator).second );
		GUI_Data.colorParsedFullDataVector.push_back({ (*colorFullDataIterator).first , ImVec4(x, y, z, w) });

		colorFullDataIterator++;
	}
}

void ColoredSquare::shuffleColorParsedFullDataVector()
{
	auto rd = std::random_device{};
	auto rng = std::default_random_engine{ rd() };

	std::shuffle(std::begin(GUI_Data.colorParsedFullDataVector), std::end(GUI_Data.colorParsedFullDataVector), rng);
}

ImVec4 ColoredSquare::createImGuiColorFromColorRGB(const std::string colorName)
{
	auto [x, y, z, w] = breakConstantColorCode(colorName);

	return ImVec4(x, y, z, w);
}

bool ColoredSquare::compareColors(const ImVec4 color1, const ImVec4 color2) const
{
	if (color1.x == color2.x &&
		color1.y == color2.y &&
		color1.z == color2.z &&
		color1.w == color2.w) {

		return true;
	}

	return false;
}

std::tuple<float, float, float, float> ColoredSquare::breakConstantColorCode(const std::string constantColorCode)
{
	std::string colorCode = constantColorCode;
	float x{}, y{}, z{}, w{};

	auto f_cutFirstFloatFromString = [&colorCode]() -> float {
		
		std::string subStr = colorCode.substr(0, colorCode.find_first_of("f"));
		float ret = std::stof(subStr, nullptr);
		colorCode.erase(0, colorCode.find_first_of(",") + 1);

		return ret;
	};

	x = f_cutFirstFloatFromString();
	y = f_cutFirstFloatFromString();
	z = f_cutFirstFloatFromString();
	w = f_cutFirstFloatFromString();

	return std::tuple<float, float, float, float>(x, y, z, w);
}

//////////////////////////////////////////////////////////////////////
// Методы отправки запросов к web-сервисам

void ColoredSquare::requestGeolocationData()
{
	std::vector<std::string> ipCheckingGeolotationServices{
		{URL_IP_API_COM},
		{URL_IP_INFO_IO}
	};

	cpr::Response response{};
	std::string country{}, region{}, city{};

	auto url_iter = ipCheckingGeolotationServices.begin();
	while (url_iter != ipCheckingGeolotationServices.end())
	{
		response = cpr::Get(cpr::Url{ *url_iter });
		if (goodResponseStatusCheck(response) == true)
			break;
		url_iter++;
	}

	if (url_iter != ipCheckingGeolotationServices.end())
	{
		hasReturnedValidResponseGeolocation = true;

		json j = json::parse(response.text);

		if (*url_iter == URL_IP_API_COM)
		{
			country = j.at("country");
			region = j.at("regionName");
			city = j.at("city");

			float lat = j.at("lat");
			float lon = j.at("lon");

			this->Location.latitude = std::to_string(lat);
			this->Location.longitude = std::to_string(lon);
		}
		else if (*url_iter == URL_IP_INFO_IO)
		{
			country = j.at("country");
			region = j.at("region");
			city = j.at("city");

			std::string loc = j.at("loc");
			size_t npos = loc.find(",");

			this->Location.latitude = loc.substr(0, npos);
			this->Location.longitude = loc.substr(npos + 1, loc.length());
		}
	}
	else
	{
		hasReturnedValidResponseGeolocation = false;
		return;
	}

	if (region == city)
		this->city = country + ":" + city;
	else
		this->city = country + ":" + region + ":" + city;
	
	std::cout << "\n" << "Place of request: " << this->city << std::endl;
	std::cout << "Coordinates: " << "latitude = " << this->Location.latitude << "; " << "longitude = " << this->Location.longitude << "\n" << std::endl;
}

void ColoredSquare::requestMeteoData()
{
	std::vector<std::string> meteoServices{
		{constructMeteoDataRequestUrl(URL_OPEN_METEO_COM)}
	};

	cpr::Response response{};

	auto url_iter = meteoServices.begin();
	while (url_iter != meteoServices.end())
	{
		response = cpr::Get(cpr::Url{ *url_iter });
		if (goodResponseStatusCheck(response) == true)
			break;
		url_iter++;
	}

	if (url_iter != meteoServices.end())
	{
		hasReturnedValidResponseMeteo = true;

		json j = json::parse(response.text);

		if ((*url_iter).find(URL_OPEN_METEO_COM) != std::string::npos)
		{
			float temperature_num = j.at("current").at("temperature_2m");
			temperature_num = roundf(temperature_num * 100)/100;

			std::string temperature_num_str = std::to_string(temperature_num);
			temperature_num_str = temperature_num_str.substr(0, temperature_num_str.find(".") + 2);

			std::string temperature_units = j.at("current_units").at("temperature_2m");

			this->temperature = temperature_num_str + temperature_units;
		}
	}
	else
	{
		hasReturnedValidResponseMeteo = false;
		return;
	}
	std::cout << "\n" << "Temperature: " << this->temperature << "\n" << std::endl;
}

void ColoredSquare::requestTimeData()
{
	std::vector<std::string> timeServices{
		{constructTimeDataRequestUrl(URL_TIMEAPI_IO)}
	};

	cpr::Response response{};
	std::string day_str{}, month_str{}, year_str{};

	auto url_iter = timeServices.begin();
	while (url_iter != timeServices.end())
	{
		response = cpr::Get(cpr::Url{ *url_iter });
		if (goodResponseStatusCheck(response) == true)
			break;
		url_iter++;
	}

	if (url_iter != timeServices.end())
	{
		hasReturnedValidResponseTime = true;

		json j = json::parse(response.text);

		if ((*url_iter).find(URL_TIMEAPI_IO) != std::string::npos)
		{
			int32_t day_num = j.at("day");
			int32_t month_num = j.at("month");
			int32_t year_num = j.at("year");

			day_str = std::to_string(day_num);
			month_str = std::to_string(month_num);
			year_str = std::to_string(year_num);

			this->date = day_str + "/" + month_str + "/" + year_str;
			this->time = j.at("time");
		}
	}
	else
	{
		hasReturnedValidResponseTime = false;
		return;
	}

	std::cout << "\n" << "Date: " << this->date << std::endl;
	std::cout << "Time: " << this->time << "\n" << std::endl;
}

std::string ColoredSquare::constructMeteoDataRequestUrl(const std::string url)
{
	std::string constructedRequest{};

	if (url == URL_OPEN_METEO_COM) {
		constructedRequest = url + "?latitude=" + Location.latitude + "&longitude=" + Location.longitude + "&current=temperature_2m" + "&timezone=auto";
	}

	return constructedRequest;
}

std::string ColoredSquare::constructTimeDataRequestUrl(const std::string url)
{
	std::string constructedRequest{};

	if (url == URL_TIMEAPI_IO) {
		constructedRequest = url + "?latitude=" + Location.latitude + "&longitude=" + Location.longitude;
	}

	return constructedRequest;
}

bool ColoredSquare::goodResponseStatusCheck(cpr::Response& response)
{
	std::cout << response.url << ": ";

	lastRequestLog = requestLogDelimiter;
	lastRequestLog += "\n" + response.url.str() + ": ";

	if (response.status_code == 0) {
		std::cout << response.error.message << std::endl;

		lastRequestLog += response.error.message;

		return false;
	}
	else if (response.status_code >= 400) {
		std::cout << response.status_code << std::endl;

		lastRequestLog += "Error: Response status code: " + response.status_code;

		return false;
	}

	if (response.header["content-type"].find("json") == std::string::npos) {
		std::string jsonNotSupportedStr = "Error: Response was sent in format that is not supported";
		std::cout << jsonNotSupportedStr << std::endl;

		lastRequestLog += jsonNotSupportedStr;

		return false;
	}

	std::string validResponse = "Valid response";
	std::cout << validResponse;

	lastRequestLog += validResponse;

	return true;
}
