#pragma once

/* Type Definition-----------------------------------------------------------*/

/* Macro Definition-----------------------------------------------------------*/

/* include Definition-----------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define AI_UI_INCLUDE				(1)
#include "ai_base_type.h"
#include "ai_base_error.h"
#include "ai_base_product.h"

/* some macro stub ---------------------------------------------------------*/
#include "ai_server_win_stub.h"

/* include Definition-----------------------------------------------------------*/
#include "ai_base_global.h"
#include "ai_net_global.h"
#include "ai_manage_global.h"
#include "ai_server_global.h"

/* include Definition-----------------------------------------------------------*/
#include "SiAi.h"
#include "SiAiSdkData.h"
#include "SiAiAlgorithm.h"
#include "SiAiTickTest.h"
#include "SiAiLogFile.h"
#include "SiAiSerialComm.h"
#include "SiAiManageCom.h"
#include "SiAiGetDeviceStateFile.h"
#include "SiAiDeviceData.h"
#include "SiAiDeviceStateNumberDataProcessBar.h"
#include "SiAiDeviceStateNumberDataSwitchBar.h"
#include "SiAiDeviceStateNameDataSwitchBar.h"
#include "SiAiDeviceStateNameData.h"
#include "SiAiDeviceConfigButton.h"
#include "SiAiDeviceConfigNumberData.h"
#include "SiAiDeviceConfigNumberDataProcessBar.h"
#include "SiAiDeviceConfigNumberDataSwitchBar.h"
#include "SiAiDeviceConfigNameDataSwitchBar.h"
#include "SiAiDeviceConfigNameDataCombo.h"
#include "SiAiDeviceConfigNameDataComboShort.h"
#include "SiAiDeviceConfigNameData.h"
#include "SiAiDeviceConfigNameDataShort.h"
#include "SiAiDeviceConfigNameDataTab.h"
#include "SiAiStateFileContent.h"
#include "SiAiStateFileF16Content.h"
#include "SiAiStateFileF32Content.h"
#include "SiAiDeviceSelectService.h"
#include "SiAiConfigIdent.h"
#include "SiAiConfigWeightsConvert.h"
#include "SiAiStateNet.h"
#include "SiAiDlg.h"
#include "SiAiHelp.h"
#include "afxdialogex.h"
#include <Dbt.h>