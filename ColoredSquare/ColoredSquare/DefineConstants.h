#pragma once

//////////////////////////////////////////////////////////////////////
// RGB цвета и их отображаемые названия

#define COLOR_NAME_WHITE		"white"
#define COLOR_NAME_BLACK		"black"
#define COLOR_NAME_BROWN		"brown"
#define COLOR_NAME_BLUE			"blue"
#define COLOR_NAME_LIGHT_BLUE	"light blue"
#define COLOR_NAME_RED			"red"
#define COLOR_NAME_YELLOW		"yellow"
#define COLOR_NAME_GREEN		"green"
#define COLOR_NAME_GREY			"grey"
#define COLOR_NAME_PINK			"pink"

#define COLOR_RGB_WHITE			"1.0f, 1.0f, 1.0f, 1.0f"
#define COLOR_RGB_BLACK			"0.1f, 0.1f, 0.1f, 1.0f"
#define COLOR_RGB_BROWN			"0.5f, 0.3f, 0.1f, 1.0f"
#define COLOR_RGB_BLUE			"0.05f, 0.1f, 0.9f, 1.0f"
#define COLOR_RGB_LIGHT_BLUE	"0.02f, 0.65f, 1.0f, 1.0f"
#define COLOR_RGB_RED			"0.8f, 0.01f, 0.01f, 1.0f"
#define COLOR_RGB_YELLOW		"0.85f, 0.85f, 0.0f, 1.0f"
#define COLOR_RGB_GREEN			"0.03f, 0.8f, 0.06f, 1.0f"
#define COLOR_RGB_GREY			"0.6f, 0.6f, 0.6f, 1.0f"
#define COLOR_RGB_PINK			"1.0f, 0.6f, 0.75f, 1.0f"


#define STYLE_TEXT_COLOR_RGB_LIGHT_GREY "0.9f, 0.9f, 0.9f, 1.0f"
#define STYLE_TEXT_COLOR_RGB_FULL_WHITE "1.0f, 1.0f, 1.0f, 1.0f"
#define STYLE_TEXT_COLOR_RGB_FULL_BLACK "0.0f, 0.0f, 0.0f, 1.0f"
#define BACKGROUND_COLOR_RGB_DARK_GREEN "0.1f, 0.3f, 0.3f, 1.0f"

//////////////////////////////////////////////////////////////////////
// Списки сервисов для определения локации, погоды и времени реализованы в виде векторов, хранящих url. 
// Это позволяет легко масштабировать код обработки ответа сервисов, что при дополнении списка уменьшит риск не получить нужные данные.

// Геолокационные web-сервисы

#define URL_IP_API_COM "http://ip-api.com/json/"
#define URL_IP_INFO_IO "https://ipinfo.io/"

// Метеорологические web-сервисы

#define URL_OPEN_METEO_COM "https://api.open-meteo.com/v1/forecast"

// Web-сервисы для определения текущего времени по координатам

#define URL_TIMEAPI_IO "https://timeapi.io/api/time/current/coordinate"


// В данный момент нашел по одному метеорологическому и проверяющему время по координатам сервису, которые не используют API ключ и не привязаны к конкретной стране.
// При желании можно легко масштабировать код под сервисы с API ключом, но это потребует регистрироваться на сервисах и указывать ключ в запросе.
//////////////////////////////////////////////////////////////////////

#define REQUEST_LOG_DELIMITER "################################################################################"