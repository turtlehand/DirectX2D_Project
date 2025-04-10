#pragma once

#include "framework.h"
#include "Client.h"

#include <crtdbg.h>
#include <Engine\\global.h>
#include <Engine\\GEngine.h>

#ifdef _DEBUG
#pragma comment(lib,"Engine\\Engine_d.lib")
#pragma comment(lib,"Practice\\Practice_d.lib");
#else
#pragma comment(lib,"Engine\\Engine.lib")
#pragma comment(lib,"Practice\\Practice.lib");


#endif // DEBUG
