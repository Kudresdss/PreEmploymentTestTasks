#pragma once

//////////////////////////////////////////////////////////////////////
// Графический интерфейс: DearImGui (GUI) + GLFW (менеджмент окна) + GLAD (инициализация OpenGL)
#include "imgui.h"
#include "imgui_impl_opengl3.h"

#include "glad.h"
#include "GLFW/glfw3.h"

#include "imgui_impl_glfw.h"

//////////////////////////////////////////////////////////////////////
// Сетевое взаимодействие с внешними сервисами: CPR (HTTP библиотека) + nlohmann_json (json парсер)
#include "cpr/cpr.h"
#define CPR_VERSION "1.11.1"

#include "nlohmann/json.hpp"

//////////////////////////////////////////////////////////////////////
// Список web-сервисов, RGB кодов цветов их отображаемых названий
#include "DefineConstants.h"

//////////////////////////////////////////////////////////////////////
// std
#include <iostream>
#include <stdexcept>

#include <math.h>
#include <algorithm>
#include <random>

#include <chrono>

//////////////////////////////////////////////////////////////////////
// Callback-функции для корректной работы GUI

static void glfw_error_callback(int error, const char* description)
{
	std::cout << "GLFW error callback: " << error << " error: " << description << std::endl;
}

static void glfw_window_size_callback(GLFWwindow* window, int width, int height) {}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

//////////////////////////////////////////////////////////////////////
// Вся программная логика реализована в классе ColoredSquare

class ColoredSquare
{
public:

	using steady_clock = std::chrono::steady_clock;
	using system_clock = std::chrono::system_clock;
	using json = nlohmann::json;

	// Доступен только дефолтный конструктор, остальные удалены за ненадобностью, как и операторы копирования (автоматически):

	ColoredSquare();
	ColoredSquare(const ColoredSquare& other) = delete;
	ColoredSquare(ColoredSquare&& other) = delete;
	~ColoredSquare();

	// Запуск программы производится через единственный вызов метода run()
	void run();

private:

	// Инициализация и подготовка графических библиотек
	void setupView();
	// Вызов основной логики
	void startView();

	// Отрисовка GUI
	void showGUI();

	//////////////////////////////////////////////////////////////////////
	// Вспомогательные методы для работы с вектором цветов квадратов и их названий

	void fillColorVectors();
	void shuffleColorParsedFullDataVector();
	ImVec4 createImGuiColorFromColorRGB(const std::string colorName);
	bool compareColors(const ImVec4 color1, const ImVec4 color2) const;

	std::tuple<float, float, float, float> breakConstantColorCode(const std::string constantColorCode);

	//////////////////////////////////////////////////////////////////////
	// Методы отправки запросов к web-сервисам

	void requestGeolocationData();
	void requestMeteoData();
	void requestTimeData();

	std::string constructMeteoDataRequestUrl(const std::string url);
	std::string constructTimeDataRequestUrl(const std::string url);

	bool goodResponseStatusCheck(cpr::Response& response);

private:

	//////////////////////////////////////////////////////////////////////
	// GUI данные

	GLFWwindow* window{};
	
	struct GUI_Data
	{
		std::vector < std::pair<std::string, std::string>> colorFullDataVector{};
		std::vector < std::pair<std::string, ImVec4>> colorParsedFullDataVector{};

		uint32_t clickOnSquaresCounter{};
	};
	GUI_Data GUI_Data{};

	//////////////////////////////////////////////////////////////////////
	// Данные из запросов к web-сервисам

	struct LocationStruct
	{
		std::string latitude;
		std::string longitude;
	};
	LocationStruct Location{};

	std::string temperature{};
	std::string city{};
	std::string date{};
	std::string time{};

	//////////////////////////////////////////////////////////////////////
	// Вспомогательные данные для отправки запросов

	steady_clock::time_point requestCheckPoint{};
	bool isRequestTimeSynchronized{ false };

	bool hasReturnedValidResponseGeolocation{ false };
	bool hasReturnedValidResponseMeteo{ false };
	bool hasReturnedValidResponseTime{ false };

	//////////////////////////////////////////////////////////////////////
	// Логирование запросов

	std::string lastRequestLog{};
	std::string requestLogDelimiter{REQUEST_LOG_DELIMITER};
	uint32_t requestCount{};
};
