#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_WARNINGS

#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAKE_NVAPI_VERSION(type, version) ((unsigned int)(sizeof(type) | ((version) << 16)))

struct NV_GPU_PERF_PSTATES20_PARAM_DELTA
{
	int value;

	struct
	{
		int min;
		int max;
	} valueRange;
};

struct NV_GPU_PSTATE20_BASE_VOLTAGE_ENTRY
{
	unsigned int domainId;
	unsigned int editable : 1;
	unsigned int reserved : 31;
	unsigned int voltageUV;
	NV_GPU_PERF_PSTATES20_PARAM_DELTA voltageDeltaUV;
};

struct NV_GPU_PSTATE20_CLOCK_ENTRY
{
	unsigned int domainId;
	unsigned int typeId;
	unsigned int editable : 1;
	unsigned int reserved : 31;
	NV_GPU_PERF_PSTATES20_PARAM_DELTA frequencyDeltaKHz;

	union
	{
		struct
		{
			unsigned int frequencyKHz;
		} single;

		struct
		{
			unsigned int minFrequencyKHz;
			unsigned int maxFrequencyKHz;
			unsigned int domainId;
			unsigned int minVoltageUV;
			unsigned int maxVoltageUV;
		} range;
	} data;
};

struct NV_GPU_PERF_PSTATES20_INFO
{
	unsigned int version;
	unsigned int editable : 1;
	unsigned int reserved : 31;
	unsigned int numPStates;
	unsigned int numClocks;
	unsigned int numBaseVoltages;

	struct
	{
		unsigned int pStateId;
		unsigned int editable : 1;
		unsigned int reserved : 31;
		NV_GPU_PSTATE20_CLOCK_ENTRY clocks[8];
		NV_GPU_PSTATE20_BASE_VOLTAGE_ENTRY baseVoltages[4];
	} pStates[16];

	struct
	{
		unsigned int numVoltages;
		NV_GPU_PSTATE20_BASE_VOLTAGE_ENTRY voltages[4];
	} overVoltage;
};

struct NV_GPU_CLOCK_LOCK
{
	unsigned int version;
	unsigned int flags;
	unsigned int count;

	struct
	{
		unsigned int index;
		unsigned int unknown1;
		unsigned int mode;
		unsigned int unknown2;
		unsigned int voltageUV;
		unsigned int unknown3;
	} entries[32];
};

struct NV_GPU_POWER_STATUS
{
	unsigned int version;
	unsigned int count;

	struct
	{
		unsigned int unknown1;
		unsigned int unknown2;
		unsigned int power;
		unsigned int unknown3;
	} entries[4];
};

struct NV_GPU_THERMAL_LIMIT
{
	unsigned int version;
	unsigned int count;

	struct
	{
		unsigned int controller;
		unsigned int value;
		unsigned int flags;
	} entries[4];
};

struct NV_GPU_COOLER_LEVEL
{
	unsigned int version;

	struct
	{
		unsigned int level;
		unsigned int policy;
	} coolers[3];
};

struct NV_GPU_SET_ILLUMINATION_PARM
{
	unsigned int version;
	void *gpuHandle;
	unsigned int attribute;
	unsigned int value;
};

struct AdapterInfo
{
	int size;
	int adapterIndex;
	char strUDID[256];
	int busNumber;
	int deviceNumber;
	int functionNumber;
	int vendorID;
	char adapterName[256];
	char displayName[256];
	int present;
	int exist;
	char driverPath[256];
	char driverPathExt[256];
	char pnpString[256];
	int osDisplayIndex;
};

struct ADLODNPerformanceLevelX2
{
	int clock;
	int vddc;
	int enabled;
	int unknown;
};

struct ADLODNPerformanceLevelsX2
{
	int size;
	int mode;
	int numberOfPerformanceLevels;
	ADLODNPerformanceLevelX2 levels[8];
};

struct ADLODNPowerLimitSetting
{
	int mode;
	int tdpLimit;
	int maxOperatingTemperature;
};

struct ADLODNFanControl
{
	int mode;
	int fanControlMode;
	int currentFanSpeedMode;
	int currentFanSpeed;
	int targetFanSpeed;
	int targetTemperature;
	int minPerformanceClock;
	int minFanLimit;
};

struct ADLFanSpeedValue
{
	int size;
	int speedType;
	int fanSpeed;
	int flags;
};

void* (__cdecl *NvAPI_QueryInterface)(unsigned int offset) = 0;
int (__cdecl *NvAPI_Initialize)() = 0;
int (__cdecl *NvAPI_Unload)() = 0;
int (__cdecl *NvAPI_RestartDisplayDriver)() = 0;
int (__cdecl *NvAPI_EnumPhysicalGPUs)(void **gpuHandles, unsigned int *gpuCount) = 0;
int (__cdecl *NvAPI_EnumTCCPhysicalGPUs)(void **gpuHandles, unsigned int *gpuCount) = 0;
int (__cdecl *NvAPI_GPU_SetPstates20)(void *gpuHandle, NV_GPU_PERF_PSTATES20_INFO *pStatesInfo) = 0;
int (__cdecl *NvAPI_GPU_GetClockBoostLock)(void *gpuHandle, NV_GPU_CLOCK_LOCK *clockLock) = 0;
int (__cdecl *NvAPI_GPU_SetClockBoostLock)(void *gpuHandle, NV_GPU_CLOCK_LOCK *clockLock) = 0;
int (__cdecl *NvAPI_GPU_ClientPowerPoliciesSetStatus)(void *gpuHandle, NV_GPU_POWER_STATUS *powerStatus) = 0;
int (__cdecl *NvAPI_GPU_ClientThermalPoliciesSetLimit)(void *gpuHandle, NV_GPU_THERMAL_LIMIT *thermalLimit) = 0;
int (__cdecl *NvAPI_GPU_SetCoolerLevels)(void *gpuHandle, unsigned int coolerIndex, NV_GPU_COOLER_LEVEL *coolerLevel) = 0;
int (__cdecl *NvAPI_GPU_SetIllumination)(NV_GPU_SET_ILLUMINATION_PARM *illuminationInfo) = 0;
int (__cdecl *NvAPI_GPU_SetPstate)(void *gpuHandle, unsigned int pState, unsigned int setType) = 0;
int (__cdecl *NvAPI_GPU_SetPstateClientLimits)(void *gpuHandle, unsigned int limitType, unsigned int pState) = 0;
int (__cdecl *NvAPI_GPU_EnableDynamicPstates)(void *gpuHandle, unsigned int dynamicPStates) = 0;
int (__cdecl *NvAPI_GPU_EnableOverclockedPstates)(void *gpuHandle, unsigned int overclockedPStates) = 0;

int (__cdecl *ADL2_Main_Control_Create)(void* (__stdcall* mallocCallback)(int size), int enumConnectedAdapters, void **context) = 0;
int (__cdecl *ADL2_Main_Control_Destroy)(void *context) = 0;
int (__cdecl *ADL2_Adapter_NumberOfAdapters_Get)(void *context, int *numberAdapters) = 0;
int (__cdecl *ADL2_Adapter_AdapterInfo_Get)(void *context, AdapterInfo *adapterInfo, int size) = 0;
int (__cdecl *ADL2_Adapter_ID_Get)(void *context, int adapterIndex, int *adapterId) = 0;
int (__cdecl *ADL2_OverdriveN_SystemClocksX2_Get)(void *context, int adapterIndex, ADLODNPerformanceLevelsX2 *performanceLevels) = 0;
int (__cdecl *ADL2_OverdriveN_SystemClocksX2_Set)(void *context, int adapterIndex, ADLODNPerformanceLevelsX2 *performanceLevels) = 0;
int (__cdecl *ADL2_OverdriveN_MemoryClocksX2_Get)(void *context, int adapterIndex, ADLODNPerformanceLevelsX2 *performanceLevels) = 0;
int (__cdecl *ADL2_OverdriveN_MemoryClocksX2_Set)(void *context, int adapterIndex, ADLODNPerformanceLevelsX2 *performanceLevels) = 0;
int (__cdecl *ADL2_OverdriveN_PowerLimit_Get)(void *context, int adapterIndex, ADLODNPowerLimitSetting *powerLimitSetting) = 0;
int (__cdecl *ADL2_OverdriveN_PowerLimit_Set)(void *context, int adapterIndex, ADLODNPowerLimitSetting *powerLimitSetting) = 0;
int (__cdecl *ADL2_OverdriveN_FanControl_Set)(void *context, int adapterIndex, ADLODNFanControl *fanControl) = 0;
int (__cdecl *ADL2_Overdrive5_FanSpeed_Set)(void *context, int adapterIndex, int thermalControllerIndex, ADLFanSpeedValue *fanSpeedValue) = 0;
int (__cdecl *ADL2_Overdrive5_FanSpeedToDefault_Set)(void *context, int adapterIndex, int thermalControllerIndex) = 0;

enum class LedType
{
	Logo = 0,
	SliBridge = 1
};

enum class PStateLimitType
{
	Soft = 1,
	Hard = 2,
	All = 3
};

void *NvApiGpuHandles[128] = {0};
unsigned int NvApiGpuCount = 0;

unsigned int AdlGpuHandles[128] = {0};
unsigned int AdlGpuCount = 0;

void *AdlContext = 0;

#define LOG(expression) Log(#expression, strrchr(__FILE__, '\\') + 1, __LINE__, (intptr_t) (expression));

FILE *LogFile = 0;

intptr_t Log(const char *expression, const char *fileName, int line, intptr_t result)
{
	if (LogFile)
	{
		time_t t = time(0);
		tm *local = localtime(&t);

		fprintf(LogFile, "[%02d.%02d.%04d %02d:%02d:%02d][%s:%d] %s = %zd (0x%0*zX)\n",
				local->tm_mday, local->tm_mon + 1, local->tm_year + 1900, local->tm_hour, local->tm_min, local->tm_sec, fileName, line, expression, result, (int) sizeof(result) * 2, result);

		fflush(LogFile);
	}

	return result;
}

int NvApiLoad()
{
	int result = -1;

	HMODULE nvapi = 0;

#if defined _WIN64
	char nvapiDllName[] = "nvapi64.dll";
#else
	char nvapiDllName[] = "nvapi.dll";
#endif

	LOG(nvapi = LoadLibraryA(nvapiDllName));

	result = !(nvapi != 0);

	if (result == 0)
	{
		LOG(NvAPI_QueryInterface = (void* (__cdecl*)(unsigned int)) GetProcAddress(nvapi, "nvapi_QueryInterface"));

		result = !(NvAPI_QueryInterface != 0);

		if (result == 0)
		{
			NvAPI_Initialize = (int (__cdecl *)()) NvAPI_QueryInterface(0x0150E828);
			NvAPI_Unload = (int (__cdecl*)()) NvAPI_QueryInterface(0xD22BDD7E);
			NvAPI_RestartDisplayDriver = (int (__cdecl*)()) NvAPI_QueryInterface(0xB4B26B65);
			NvAPI_EnumPhysicalGPUs = (int (__cdecl*)(void**, unsigned int*)) NvAPI_QueryInterface(0xE5AC921F);
			NvAPI_EnumTCCPhysicalGPUs = (int (__cdecl*)(void**, unsigned int*)) NvAPI_QueryInterface(0xD9930B07);
			NvAPI_GPU_SetPstates20 = (int (__cdecl*)(void*, NV_GPU_PERF_PSTATES20_INFO*)) NvAPI_QueryInterface(0x0F4DAE6B);
			NvAPI_GPU_GetClockBoostLock = (int (__cdecl*)(void*, NV_GPU_CLOCK_LOCK*)) NvAPI_QueryInterface(0xE440B867);
			NvAPI_GPU_SetClockBoostLock = (int (__cdecl*)(void*, NV_GPU_CLOCK_LOCK*)) NvAPI_QueryInterface(0x39442CFB);
			NvAPI_GPU_ClientPowerPoliciesSetStatus = (int (__cdecl*)(void*, NV_GPU_POWER_STATUS*)) NvAPI_QueryInterface(0xAD95F5ED);
			NvAPI_GPU_ClientThermalPoliciesSetLimit = (int (__cdecl*)(void*, NV_GPU_THERMAL_LIMIT*)) NvAPI_QueryInterface(0x34C0B13D);
			NvAPI_GPU_SetCoolerLevels = (int (__cdecl*)(void*, unsigned int, NV_GPU_COOLER_LEVEL*)) NvAPI_QueryInterface(0x891FA0AE);
			NvAPI_GPU_SetIllumination = (int (__cdecl*)(NV_GPU_SET_ILLUMINATION_PARM*)) NvAPI_QueryInterface(0x0254A187);
			NvAPI_GPU_SetPstate = (int (__cdecl*)(void*, unsigned int, unsigned int)) NvAPI_QueryInterface(0x025BFB10);
			NvAPI_GPU_SetPstateClientLimits = (int (__cdecl*)(void*, unsigned int, unsigned int)) NvAPI_QueryInterface(0xFDFC7D49);
			NvAPI_GPU_EnableDynamicPstates = (int (__cdecl*)(void*, unsigned int)) NvAPI_QueryInterface(0xFA579A0F);
			NvAPI_GPU_EnableOverclockedPstates = (int (__cdecl*)(void*, unsigned int)) NvAPI_QueryInterface(0xB23B70EE);
		}
	}

	return result;
}

int NvApiInit()
{
	int result = -1;

	if (NvAPI_Initialize)
	{
		LOG(result = NvAPI_Initialize());
	}

	return result;
}

int NvApiFree()
{
	int result = -1;

	if (NvAPI_Unload)
	{
		LOG(result = NvAPI_Unload());
	}

	return result;
}

int NvApiRestartDriver()
{
	int result = -1;

	if (NvAPI_RestartDisplayDriver)
	{
		LOG(result = NvAPI_RestartDisplayDriver());
	}

	return result;
}

int NvApiEnumGpus()
{
	int result = -1;

	if (NvAPI_EnumPhysicalGPUs)
	{
		unsigned int count = 0;

		LOG(result = NvAPI_EnumPhysicalGPUs(NvApiGpuHandles + NvApiGpuCount, &count));

		NvApiGpuCount += count;
	}

	return result;
}

int NvApiEnumTccGpus()
{
	int result = -1;

	if (NvAPI_EnumTCCPhysicalGPUs)
	{
		unsigned int count = 0;

		LOG(result = NvAPI_EnumTCCPhysicalGPUs(NvApiGpuHandles + NvApiGpuCount, &count));

		NvApiGpuCount += count;
	}

	return result;
}

int NvApiSetCoreClockOffset(unsigned int gpuIndex, unsigned int pState, int frequencyOffsetKHz)
{
	int result = -1;

	if (NvAPI_GPU_SetPstates20)
	{
		if (gpuIndex < NvApiGpuCount)
		{
			NV_GPU_PERF_PSTATES20_INFO pStatesInfo = {0};

			pStatesInfo.version = MAKE_NVAPI_VERSION(pStatesInfo, 2);
			pStatesInfo.numPStates = 1;
			pStatesInfo.numClocks = 1;
			pStatesInfo.pStates[0].pStateId = pState;
			pStatesInfo.pStates[0].clocks[0].domainId = 0;
			pStatesInfo.pStates[0].clocks[0].typeId = 0;
			pStatesInfo.pStates[0].clocks[0].frequencyDeltaKHz.value = frequencyOffsetKHz;

			LOG(result = NvAPI_GPU_SetPstates20(NvApiGpuHandles[gpuIndex], &pStatesInfo));
		}
	}

	return result;
}

int NvApiSetMemoryClockOffset(unsigned int gpuIndex, unsigned int pState, int frequencyOffsetKHz)
{
	int result = -1;

	if (NvAPI_GPU_SetPstates20)
	{
		if (gpuIndex < NvApiGpuCount)
		{
			NV_GPU_PERF_PSTATES20_INFO pStatesInfo = {0};

			pStatesInfo.version = MAKE_NVAPI_VERSION(pStatesInfo, 2);
			pStatesInfo.numPStates = 1;
			pStatesInfo.numClocks = 1;
			pStatesInfo.pStates[0].pStateId = pState;
			pStatesInfo.pStates[0].clocks[0].domainId = 4;
			pStatesInfo.pStates[0].clocks[0].typeId = 0;
			pStatesInfo.pStates[0].clocks[0].frequencyDeltaKHz.value = frequencyOffsetKHz;

			LOG(result = NvAPI_GPU_SetPstates20(NvApiGpuHandles[gpuIndex], &pStatesInfo));
		}
	}

	return result;
}

int NvApiSetVoltageOffset(unsigned int gpuIndex, unsigned int pState, int voltageOffsetUV)
{
	int result = -1;

	if (NvAPI_GPU_SetPstates20)
	{
		if (gpuIndex < NvApiGpuCount)
		{
			NV_GPU_PERF_PSTATES20_INFO pStatesInfo = {0};

			pStatesInfo.version = MAKE_NVAPI_VERSION(pStatesInfo, 2);
			pStatesInfo.numPStates = 1;
			pStatesInfo.numBaseVoltages = 1;
			pStatesInfo.pStates[0].pStateId = pState;
			pStatesInfo.pStates[0].baseVoltages[0].domainId = 0;
			pStatesInfo.pStates[0].baseVoltages[0].voltageDeltaUV.value = voltageOffsetUV;

			LOG(result = NvAPI_GPU_SetPstates20(NvApiGpuHandles[gpuIndex], &pStatesInfo));
		}
	}

	return result;
}

int NvApiSetOverVoltageOffset(unsigned int gpuIndex, int voltageOffsetUV)
{
	int result = -1;

	if (NvAPI_GPU_SetPstates20)
	{
		if (gpuIndex < NvApiGpuCount)
		{
			NV_GPU_PERF_PSTATES20_INFO pStatesInfo = {0};

			pStatesInfo.version = MAKE_NVAPI_VERSION(pStatesInfo, 2);
			pStatesInfo.overVoltage.numVoltages = 1;
			pStatesInfo.overVoltage.voltages[0].domainId = 0;
			pStatesInfo.overVoltage.voltages[0].voltageDeltaUV.value = voltageOffsetUV;

			LOG(result = NvAPI_GPU_SetPstates20(NvApiGpuHandles[gpuIndex], &pStatesInfo));
		}
	}

	return result;
}

int NvApiSetVoltageLock(unsigned int gpuIndex, unsigned int voltageUV)
{
	int result = -1;

	if ((NvAPI_GPU_GetClockBoostLock) && (NvAPI_GPU_SetClockBoostLock))
	{
		if (gpuIndex < NvApiGpuCount)
		{
			NV_GPU_CLOCK_LOCK clockLock = {0};

			clockLock.version = MAKE_NVAPI_VERSION(clockLock, 1);

			LOG(result = NvAPI_GPU_GetClockBoostLock(NvApiGpuHandles[gpuIndex], &clockLock));

			if (result == 0)
			{
				for (unsigned int i = 0; i < clockLock.count; ++i)
				{
					if (clockLock.entries[i].index == 6)
					{
						clockLock.entries[i].mode = (voltageUV != 0) ? 3 : 0;
						clockLock.entries[i].voltageUV = voltageUV;
						clockLock.entries[i].unknown3 = 0;
					}
				}

				LOG(result = NvAPI_GPU_SetClockBoostLock(NvApiGpuHandles[gpuIndex], &clockLock));
			}
		}
	}

	return result;
}

int NvApiSetPowerLimit(unsigned int gpuIndex, unsigned int power)
{
	int result = -1;

	if (NvAPI_GPU_ClientPowerPoliciesSetStatus)
	{
		if (gpuIndex < NvApiGpuCount)
		{
			NV_GPU_POWER_STATUS powerStatus = {0};

			powerStatus.version = MAKE_NVAPI_VERSION(powerStatus, 1);
			powerStatus.count = 1;
			powerStatus.entries[0].unknown1 = 0;
			powerStatus.entries[0].power = power * 1000;

			LOG(result = NvAPI_GPU_ClientPowerPoliciesSetStatus(NvApiGpuHandles[gpuIndex], &powerStatus));
		}
	}

	return result;
}

int NvApiSetTempLimit(unsigned int gpuIndex, bool priority, unsigned int tempC)
{
	int result = -1;

	if (NvAPI_GPU_ClientThermalPoliciesSetLimit)
	{
		if (gpuIndex < NvApiGpuCount)
		{
			NV_GPU_THERMAL_LIMIT thermalLimit = {0};

			thermalLimit.version = MAKE_NVAPI_VERSION(thermalLimit, 2);
			thermalLimit.count = 1;
			thermalLimit.entries[0].controller = 1;
			thermalLimit.entries[0].value = tempC << 8;
			thermalLimit.entries[0].flags = priority ? 1 : 0;

			LOG(result = NvAPI_GPU_ClientThermalPoliciesSetLimit(NvApiGpuHandles[gpuIndex], &thermalLimit));
		}
	}

	return result;
}

int NvApiSetFanSpeed(unsigned int gpuIndex, unsigned int fanIndex, int speed)
{
	int result = -1;

	if (NvAPI_GPU_SetCoolerLevels)
	{
		if (gpuIndex < NvApiGpuCount)
		{
			NV_GPU_COOLER_LEVEL coolerLevel = {0};

			coolerLevel.version = MAKE_NVAPI_VERSION(coolerLevel, 1);
			coolerLevel.coolers[0].level = (speed >= 0) ? speed : 30;
			coolerLevel.coolers[0].policy = (speed >= 0) ? 1 : 32;

			LOG(result = NvAPI_GPU_SetCoolerLevels(NvApiGpuHandles[gpuIndex], fanIndex, &coolerLevel));
		}
	}

	return result;
}

int NvApiSetLedBrightness(unsigned int gpuIndex, LedType ledType, unsigned int brightness)
{
	int result = -1;

	if (NvAPI_GPU_SetIllumination)
	{
		if (gpuIndex < NvApiGpuCount)
		{
			NV_GPU_SET_ILLUMINATION_PARM illuminationInfo = {0};

			illuminationInfo.version = MAKE_NVAPI_VERSION(illuminationInfo, 1);
			illuminationInfo.gpuHandle = NvApiGpuHandles[gpuIndex];
			illuminationInfo.attribute = (unsigned int) ledType;
			illuminationInfo.value = brightness;

			LOG(result = NvAPI_GPU_SetIllumination(&illuminationInfo));
		}
	}

	return result;
}

int NvApiSetPState(unsigned int gpuIndex, unsigned int pState)
{
	int result = -1;

	if (NvAPI_GPU_SetPstate)
	{
		if (gpuIndex < NvApiGpuCount)
		{
			LOG(result = NvAPI_GPU_SetPstate(NvApiGpuHandles[gpuIndex], pState, 2));
		}
	}

	return result;
}

int NvApiSetPStateLimit(unsigned int gpuIndex, PStateLimitType limitType, unsigned int pState)
{
	int result = -1;

	if (NvAPI_GPU_SetPstateClientLimits)
	{
		if (gpuIndex < NvApiGpuCount)
		{
			LOG(result = NvAPI_GPU_SetPstateClientLimits(NvApiGpuHandles[gpuIndex], (unsigned int) limitType, pState));
		}
	}

	return result;
}

int NvApiEnableDynamicPStates(unsigned int gpuIndex, bool enabled)
{
	int result = -1;

	if (NvAPI_GPU_EnableDynamicPstates)
	{
		if (gpuIndex < NvApiGpuCount)
		{
			LOG(result = NvAPI_GPU_EnableDynamicPstates(NvApiGpuHandles[gpuIndex], enabled ? 1 : 0));
		}
	}

	return result;
}

int NvApiEnableOverclockedPStates(unsigned int gpuIndex, bool enabled)
{
	int result = -1;

	if (NvAPI_GPU_EnableOverclockedPstates)
	{
		if (gpuIndex < NvApiGpuCount)
		{
			LOG(result = NvAPI_GPU_EnableOverclockedPstates(NvApiGpuHandles[gpuIndex], enabled ? 1 : 0));
		}
	}

	return result;
}

int AdlLoad()
{
	int result = -1;

	HMODULE adl = 0;

	LOG(adl = LoadLibraryA("atiadlxx.dll"));

	if (adl == 0)
		LOG(adl = LoadLibraryA("atiadlxy.dll"));

	result = !(adl != 0);

	if (result == 0)
	{
		ADL2_Main_Control_Create = (int (__cdecl*)(void* (__stdcall*)(int), int, void**)) GetProcAddress(adl, "ADL2_Main_Control_Create");
		ADL2_Main_Control_Destroy = (int (__cdecl*)(void*)) GetProcAddress(adl, "ADL2_Main_Control_Destroy");
		ADL2_Adapter_NumberOfAdapters_Get = (int (__cdecl*)(void*, int*)) GetProcAddress(adl, "ADL2_Adapter_NumberOfAdapters_Get");
		ADL2_Adapter_AdapterInfo_Get = (int (__cdecl*)(void*, AdapterInfo*, int)) GetProcAddress(adl, "ADL2_Adapter_AdapterInfo_Get");
		ADL2_Adapter_ID_Get = (int (__cdecl*)(void*, int, int*)) GetProcAddress(adl, "ADL2_Adapter_ID_Get");
		ADL2_OverdriveN_SystemClocksX2_Get = (int (__cdecl*)(void*, int, ADLODNPerformanceLevelsX2*)) GetProcAddress(adl, "ADL2_OverdriveN_SystemClocksX2_Get");
		ADL2_OverdriveN_SystemClocksX2_Set = (int (__cdecl*)(void*, int, ADLODNPerformanceLevelsX2*)) GetProcAddress(adl, "ADL2_OverdriveN_SystemClocksX2_Set");
		ADL2_OverdriveN_MemoryClocksX2_Get = (int (__cdecl*)(void*, int, ADLODNPerformanceLevelsX2*)) GetProcAddress(adl, "ADL2_OverdriveN_MemoryClocksX2_Get");
		ADL2_OverdriveN_MemoryClocksX2_Set = (int (__cdecl*)(void*, int, ADLODNPerformanceLevelsX2*)) GetProcAddress(adl, "ADL2_OverdriveN_MemoryClocksX2_Set");
		ADL2_OverdriveN_PowerLimit_Get = (int (__cdecl*)(void*, int, ADLODNPowerLimitSetting*)) GetProcAddress(adl, "ADL2_OverdriveN_PowerLimit_Get");
		ADL2_OverdriveN_PowerLimit_Set = (int (__cdecl*)(void*, int, ADLODNPowerLimitSetting*)) GetProcAddress(adl, "ADL2_OverdriveN_PowerLimit_Set");
		ADL2_OverdriveN_FanControl_Set = (int (__cdecl*)(void*, int, ADLODNFanControl*)) GetProcAddress(adl, "ADL2_OverdriveN_FanControl_Set");
		ADL2_Overdrive5_FanSpeed_Set = (int (__cdecl*)(void*, int, int, ADLFanSpeedValue*)) GetProcAddress(adl, "ADL2_Overdrive5_FanSpeed_Set");
		ADL2_Overdrive5_FanSpeedToDefault_Set = (int (__cdecl*)(void*, int, int)) GetProcAddress(adl, "ADL2_Overdrive5_FanSpeedToDefault_Set");
	}

	return result;
}

void* __stdcall ADL2_Main_Memory_Alloc(int size)
{
	return malloc(size);
}

int AdlInit()
{
	int result = -1;

	if (ADL2_Main_Control_Create)
	{
		LOG(result = ADL2_Main_Control_Create(ADL2_Main_Memory_Alloc, 1, &AdlContext));
	}

	return result;
}

int AdlFree()
{
	int result = -1;

	if (ADL2_Main_Control_Destroy)
	{
		if (AdlContext)
		{
			LOG(result = ADL2_Main_Control_Destroy(AdlContext));
		}
	}

	return result;
}

int AdlEnumGpus()
{
	int result = -1;

	if ((ADL2_Adapter_NumberOfAdapters_Get) && (ADL2_Adapter_AdapterInfo_Get) && (ADL2_Adapter_ID_Get))
	{
		if (AdlContext)
		{
			int count = 0;

			LOG(result = ADL2_Adapter_NumberOfAdapters_Get(AdlContext, &count));

			if (result == 0)
			{
				if (count > 0)
				{
					int size = sizeof(AdapterInfo) * count;

					AdapterInfo *adaptersInfo = 0;

					LOG(adaptersInfo = (AdapterInfo*) malloc(size));

					result = !(adaptersInfo != 0);

					if (result == 0)
					{
						memset(adaptersInfo, 0, size);

						LOG(result = ADL2_Adapter_AdapterInfo_Get(AdlContext, adaptersInfo, size));

						if (result == 0)
						{
							int ids[128] = {0};

							for (int i = 0; i < count; ++i)
							{
								int id = 0, idResult = 0;

								LOG(idResult = ADL2_Adapter_ID_Get(AdlContext, adaptersInfo[i].adapterIndex, &id));

								if (idResult == 0)
								{
									bool idFound = false;

									for (unsigned int j = 0; j < AdlGpuCount; ++j)
									{
										if (ids[j] == id)
										{
											idFound = true;

											break;
										}
									}

									if (!idFound)
									{
										ids[AdlGpuCount] = id;

										AdlGpuHandles[AdlGpuCount] = adaptersInfo[i].adapterIndex;

										++AdlGpuCount;
									}
								}
							}
						}

						free(adaptersInfo);
					}
				}
			}
		}
	}

	return result;
}

int AdlSetCoreClock(unsigned int gpuIndex, unsigned int pState, unsigned int frequencyKHz)
{
	int result = -1;

	if ((ADL2_OverdriveN_SystemClocksX2_Get) && (ADL2_OverdriveN_SystemClocksX2_Set))
	{
		if (AdlContext)
		{
			if (gpuIndex < AdlGpuCount)
			{
				ADLODNPerformanceLevelsX2 performanceLevels = {0};

				performanceLevels.size = sizeof(performanceLevels);
				performanceLevels.numberOfPerformanceLevels = 8;

				LOG(result = ADL2_OverdriveN_SystemClocksX2_Get(AdlContext, AdlGpuHandles[gpuIndex], &performanceLevels));

				if (result == 0)
				{
					performanceLevels.mode = (frequencyKHz != 0) ? 3 : 1;

					for (int i = 0; i < performanceLevels.numberOfPerformanceLevels; ++i)
					{
						performanceLevels.levels[i].enabled = ((performanceLevels.levels[i].enabled & (1 << i)) != 0);
						performanceLevels.levels[i].unknown = 0;

						if (i == pState)
							performanceLevels.levels[pState].clock = frequencyKHz / 10;
					}

					LOG(result = ADL2_OverdriveN_SystemClocksX2_Set(AdlContext, AdlGpuHandles[gpuIndex], &performanceLevels));
				}
			}
		}
	}

	return result;
}

int AdlSetCoreVoltage(unsigned int gpuIndex, unsigned int pState, unsigned int voltageUV)
{
	int result = -1;

	if ((ADL2_OverdriveN_SystemClocksX2_Get) && (ADL2_OverdriveN_SystemClocksX2_Set))
	{
		if (AdlContext)
		{
			if (gpuIndex < AdlGpuCount)
			{
				ADLODNPerformanceLevelsX2 performanceLevels = {0};

				performanceLevels.size = sizeof(performanceLevels);
				performanceLevels.numberOfPerformanceLevels = 8;

				LOG(result = ADL2_OverdriveN_SystemClocksX2_Get(AdlContext, AdlGpuHandles[gpuIndex], &performanceLevels));

				if (result == 0)
				{
					performanceLevels.mode = (voltageUV != 0) ? 3 : 1;

					for (int i = 0; i < performanceLevels.numberOfPerformanceLevels; ++i)
					{
						performanceLevels.levels[i].enabled = ((performanceLevels.levels[i].enabled & (1 << i)) != 0);
						performanceLevels.levels[i].unknown = 0;

						if (i == pState)
							performanceLevels.levels[pState].vddc = voltageUV / 1000;
					}

					LOG(result = ADL2_OverdriveN_SystemClocksX2_Set(AdlContext, AdlGpuHandles[gpuIndex], &performanceLevels));
				}
			}
		}
	}

	return result;
}

int AdlEnableCorePState(unsigned int gpuIndex, unsigned int pState, bool enabled)
{
	int result = -1;

	if ((ADL2_OverdriveN_SystemClocksX2_Get) && (ADL2_OverdriveN_SystemClocksX2_Set))
	{
		if (AdlContext)
		{
			if (gpuIndex < AdlGpuCount)
			{
				ADLODNPerformanceLevelsX2 performanceLevels = {0};

				performanceLevels.size = sizeof(performanceLevels);
				performanceLevels.numberOfPerformanceLevels = 8;

				LOG(result = ADL2_OverdriveN_SystemClocksX2_Get(AdlContext, AdlGpuHandles[gpuIndex], &performanceLevels));

				if (result == 0)
				{
					performanceLevels.mode = 3;

					for (int i = 0; i < performanceLevels.numberOfPerformanceLevels; ++i)
					{
						performanceLevels.levels[i].enabled = ((performanceLevels.levels[i].enabled & (1 << i)) != 0);
						performanceLevels.levels[i].unknown = 0;

						if (i == pState)
							performanceLevels.levels[i].enabled = enabled ? 1 : 0;
					}

					LOG(result = ADL2_OverdriveN_SystemClocksX2_Set(AdlContext, AdlGpuHandles[gpuIndex], &performanceLevels));
				}
			}
		}
	}

	return result;
}

int AdlSetMemoryClock(unsigned int gpuIndex, unsigned int pState, unsigned int frequencyKHz)
{
	int result = -1;

	if ((ADL2_OverdriveN_MemoryClocksX2_Get) && (ADL2_OverdriveN_MemoryClocksX2_Set))
	{
		if (AdlContext)
		{
			if (gpuIndex < AdlGpuCount)
			{
				ADLODNPerformanceLevelsX2 performanceLevels = {0};

				performanceLevels.size = sizeof(performanceLevels);
				performanceLevels.numberOfPerformanceLevels = 8;

				LOG(result = ADL2_OverdriveN_MemoryClocksX2_Get(AdlContext, AdlGpuHandles[gpuIndex], &performanceLevels));

				if (result == 0)
				{
					performanceLevels.mode = (frequencyKHz != 0) ? 3 : 1;

					for (int i = 0; i < performanceLevels.numberOfPerformanceLevels; ++i)
					{
						performanceLevels.levels[i].enabled = ((performanceLevels.levels[i].enabled & (1 << i)) != 0);
						performanceLevels.levels[i].unknown = 0;

						if (i == pState)
							performanceLevels.levels[pState].clock = frequencyKHz / 10;
					}

					LOG(result = ADL2_OverdriveN_MemoryClocksX2_Set(AdlContext, AdlGpuHandles[gpuIndex], &performanceLevels));
				}
			}
		}
	}

	return result;
}

int AdlSetMemoryVoltage(unsigned int gpuIndex, unsigned int pState, unsigned int voltageUV)
{
	int result = -1;

	if ((ADL2_OverdriveN_MemoryClocksX2_Get) && (ADL2_OverdriveN_MemoryClocksX2_Set))
	{
		if (AdlContext)
		{
			if (gpuIndex < AdlGpuCount)
			{
				ADLODNPerformanceLevelsX2 performanceLevels = {0};

				performanceLevels.size = sizeof(performanceLevels);
				performanceLevels.numberOfPerformanceLevels = 8;

				LOG(result = ADL2_OverdriveN_MemoryClocksX2_Get(AdlContext, AdlGpuHandles[gpuIndex], &performanceLevels));

				if (result == 0)
				{
					performanceLevels.mode = (voltageUV != 0) ? 3 : 1;

					for (int i = 0; i < performanceLevels.numberOfPerformanceLevels; ++i)
					{
						performanceLevels.levels[i].enabled = ((performanceLevels.levels[i].enabled & (1 << i)) != 0);
						performanceLevels.levels[i].unknown = 0;

						if (i == pState)
							performanceLevels.levels[pState].vddc = voltageUV / 1000;
					}

					LOG(result = ADL2_OverdriveN_MemoryClocksX2_Set(AdlContext, AdlGpuHandles[gpuIndex], &performanceLevels));
				}
			}
		}
	}

	return result;
}

int AdlEnableMemoryPState(unsigned int gpuIndex, unsigned int pState, bool enabled)
{
	int result = -1;

	if ((ADL2_OverdriveN_MemoryClocksX2_Get) && (ADL2_OverdriveN_MemoryClocksX2_Set))
	{
		if (AdlContext)
		{
			if (gpuIndex < AdlGpuCount)
			{
				ADLODNPerformanceLevelsX2 performanceLevels = {0};

				performanceLevels.size = sizeof(performanceLevels);
				performanceLevels.numberOfPerformanceLevels = 8;

				LOG(result = ADL2_OverdriveN_MemoryClocksX2_Get(AdlContext, AdlGpuHandles[gpuIndex], &performanceLevels));

				if (result == 0)
				{
					performanceLevels.mode = 3;

					for (int i = 0; i < performanceLevels.numberOfPerformanceLevels; ++i)
					{
						performanceLevels.levels[i].enabled = ((performanceLevels.levels[i].enabled & (1 << i)) != 0);
						performanceLevels.levels[i].unknown = 0;

						if (i == pState)
							performanceLevels.levels[i].enabled = enabled ? 1 : 0;
					}

					LOG(result = ADL2_OverdriveN_MemoryClocksX2_Set(AdlContext, AdlGpuHandles[gpuIndex], &performanceLevels));
				}
			}
		}
	}

	return result;
}

int AdlSetPowerLimit(unsigned int gpuIndex, int power)
{
	int result = -1;

	if ((ADL2_OverdriveN_PowerLimit_Get) && (ADL2_OverdriveN_PowerLimit_Set))
	{
		if (AdlContext)
		{
			if (gpuIndex < AdlGpuCount)
			{
				ADLODNPowerLimitSetting powerLimitSettings = {0};

				LOG(result = ADL2_OverdriveN_PowerLimit_Get(AdlContext, AdlGpuHandles[gpuIndex], &powerLimitSettings));

				if (result == 0)
				{
					powerLimitSettings.mode = 3;
					powerLimitSettings.tdpLimit = power;

					LOG(result = ADL2_OverdriveN_PowerLimit_Set(AdlContext, AdlGpuHandles[gpuIndex], &powerLimitSettings));
				}
			}
		}
	}

	return result;
}

int AdlSetTempLimit(unsigned int gpuIndex, unsigned int tempC)
{
	int result = -1;

	if ((ADL2_OverdriveN_PowerLimit_Get) && (ADL2_OverdriveN_PowerLimit_Set))
	{
		if (AdlContext)
		{
			if (gpuIndex < AdlGpuCount)
			{
				ADLODNPowerLimitSetting powerLimitSettings = {0};

				LOG(result = ADL2_OverdriveN_PowerLimit_Get(AdlContext, AdlGpuHandles[gpuIndex], &powerLimitSettings));

				if (result == 0)
				{
					powerLimitSettings.mode = 3;
					powerLimitSettings.maxOperatingTemperature = tempC;

					LOG(result = ADL2_OverdriveN_PowerLimit_Set(AdlContext, AdlGpuHandles[gpuIndex], &powerLimitSettings));
				}
			}
		}
	}

	return result;
}

int AdlSetFanControl(unsigned int gpuIndex, unsigned int minRPM, unsigned int maxRPM, unsigned int targetTempC, unsigned int acousticLimitKHz)
{
	int result = -1;

	if (ADL2_OverdriveN_FanControl_Set)
	{
		if (AdlContext)
		{
			if (gpuIndex < AdlGpuCount)
			{
				ADLODNFanControl fanControl = {0};

				fanControl.mode = 3;
				fanControl.fanControlMode = 0;
				fanControl.currentFanSpeedMode = 0;
				fanControl.currentFanSpeed = 0;
				fanControl.targetFanSpeed = maxRPM;
				fanControl.targetTemperature = targetTempC;
				fanControl.minPerformanceClock = acousticLimitKHz / 1000;
				fanControl.minFanLimit = minRPM;

				LOG(result = ADL2_OverdriveN_FanControl_Set(AdlContext, AdlGpuHandles[gpuIndex], &fanControl));
			}
		}
	}

	return result;
}

int AdlSetFanSpeed(unsigned int gpuIndex, unsigned int fanIndex, unsigned int speed)
{
	int result = -1;

	if (ADL2_Overdrive5_FanSpeed_Set)
	{
		if (AdlContext)
		{
			if (gpuIndex < AdlGpuCount)
			{
				ADLFanSpeedValue fanSpeedValue = {0};

				fanSpeedValue.size = sizeof(fanSpeedValue);
				fanSpeedValue.speedType = 1;
				fanSpeedValue.fanSpeed = speed;
				fanSpeedValue.flags = 1;

				LOG(result = ADL2_Overdrive5_FanSpeed_Set(AdlContext, AdlGpuHandles[gpuIndex], fanIndex, &fanSpeedValue));
			}
		}
	}

	return result;
}

int AdlSetDefaultFanSpeed(unsigned int gpuIndex, unsigned int fanIndex)
{
	int result = -1;

	if (ADL2_Overdrive5_FanSpeedToDefault_Set)
	{
		if (AdlContext)
		{
			if (gpuIndex < AdlGpuCount)
			{
				LOG(result = ADL2_Overdrive5_FanSpeedToDefault_Set(AdlContext, AdlGpuHandles[gpuIndex], fanIndex));
			}
		}
	}

	return result;
}

void PrintUsage()
{
	printf(
		"\n"
		"Usage:\n"
		"-core gpuIndex pState frequencyKHz (0 = default NVIDIA offset)\n"
		"-mem gpuIndex pState frequencyKHz (0 = default NVIDIA offset)\n"
		"-cvolt gpuIndex pState voltageUV (0 = default)\n"
		"-mvolt gpuIndex pState voltageUV (0 = default)\n"
		"-power gpuIndex power (AMD offset)\n"
		"-temp gpuIndex priority tempC (0 = false 1 = true)\n"
		"-fan gpuIndex fanIndex level (-1 = auto)\n"
		"-led gpuIndex type brightness (0 = logo 1 = sliBridge)\n"
		"-restart\n");
}

void ParseArgs(int argc, char *argv[])
{
	int arg = 1;

	while (arg < argc)
	{
		if (strcmp(strupr(argv[arg]), "-CORE") == 0)
		{
			if (arg + 3 < argc)
			{
				unsigned int gpuIndex = atoi(argv[++arg]);
				unsigned int pState = atoi(argv[++arg]);
				int frequencyKHz = atoi(argv[++arg]);

				if (gpuIndex < NvApiGpuCount)
					NvApiSetCoreClockOffset(gpuIndex, pState, frequencyKHz);
				else
					AdlSetCoreClock(gpuIndex - NvApiGpuCount, pState, frequencyKHz);
			}
		}
		else if (strcmp(strupr(argv[arg]), "-MEM") == 0)
		{
			if (arg + 3 < argc)
			{
				unsigned int gpuIndex = atoi(argv[++arg]);
				unsigned int pState = atoi(argv[++arg]);
				int frequencyKHz = atoi(argv[++arg]);

				if (gpuIndex < NvApiGpuCount)
					NvApiSetMemoryClockOffset(gpuIndex, pState, frequencyKHz);
				else
					AdlSetMemoryClock(gpuIndex - NvApiGpuCount, pState, frequencyKHz);
			}
		}
		else if (strcmp(strupr(argv[arg]), "-CVOLT") == 0)
		{
			if (arg + 3 < argc)
			{
				unsigned int gpuIndex = atoi(argv[++arg]);
				unsigned int pState = atoi(argv[++arg]);
				unsigned int voltageUV = atoi(argv[++arg]);

				if (gpuIndex < NvApiGpuCount)
					NvApiSetVoltageLock(gpuIndex, voltageUV);
				else
					AdlSetCoreVoltage(gpuIndex - NvApiGpuCount, pState, voltageUV);
			}
		}
		else if (strcmp(strupr(argv[arg]), "-MVOLT") == 0)
		{
			if (arg + 3 < argc)
			{
				unsigned int gpuIndex = atoi(argv[++arg]);
				unsigned int pState = atoi(argv[++arg]);
				unsigned int voltageUV = atoi(argv[++arg]);

				if (gpuIndex < NvApiGpuCount)
					NvApiSetVoltageLock(gpuIndex, voltageUV);
				else
					AdlSetMemoryVoltage(gpuIndex - NvApiGpuCount, pState, voltageUV);
			}
		}
		else if (strcmp(strupr(argv[arg]), "-POWER") == 0)
		{
			if (arg + 2 < argc)
			{
				unsigned int gpuIndex = atoi(argv[++arg]);
				int power = atoi(argv[++arg]);

				if (gpuIndex < NvApiGpuCount)
					NvApiSetPowerLimit(gpuIndex, power);
				else
					AdlSetPowerLimit(gpuIndex - NvApiGpuCount, power);
			}
		}
		else if (strcmp(strupr(argv[arg]), "-TEMP") == 0)
		{
			if (arg + 3 < argc)
			{
				unsigned int gpuIndex = atoi(argv[++arg]);
				unsigned int priority = atoi(argv[++arg]);
				unsigned int tempC = atoi(argv[++arg]);

				if (gpuIndex < NvApiGpuCount)
					NvApiSetTempLimit(gpuIndex, (priority != 0), tempC);
				else
					AdlSetTempLimit(gpuIndex - NvApiGpuCount, tempC);
			}
		}
		else if (strcmp(strupr(argv[arg]), "-FAN") == 0)
		{
			if (arg + 3 < argc)
			{
				unsigned int gpuIndex = atoi(argv[++arg]);
				unsigned int fanIndex = atoi(argv[++arg]);
				int speed = atoi(argv[++arg]);

				if (gpuIndex < NvApiGpuCount)
				{
					NvApiSetFanSpeed(gpuIndex, fanIndex, speed);
				}
				else
				{
					if (speed >= 0)
						AdlSetFanSpeed(gpuIndex - NvApiGpuCount, fanIndex, speed);
					else
						AdlSetDefaultFanSpeed(gpuIndex - NvApiGpuCount, fanIndex);
				}
			}
		}
		else if (strcmp(strupr(argv[arg]), "-LED") == 0)
		{
			if (arg + 3 < argc)
			{
				unsigned int gpuIndex = atoi(argv[++arg]);
				unsigned int type = atoi(argv[++arg]);
				unsigned int brightness = atoi(argv[++arg]);

				if (gpuIndex < NvApiGpuCount)
					NvApiSetLedBrightness(gpuIndex, (LedType) type, brightness);
			}
		}
		else if (strcmp(strupr(argv[arg]), "-RESTART") == 0)
		{
			NvApiRestartDriver();
		}
		else
		{
			printf("\n"
				   "Invalid parameter #%d \"%s\"\n", arg, argv[arg]);
		}

		++arg;
	}
}

int main(int argc, char *argv[])
{
	LogFile = fopen("log.txt", "a");

	if (argc > 1)
	{
		bool nvapi = false, adl = false;

		if (NvApiLoad() == 0)
		{
			if (NvApiInit() == 0)
			{
				nvapi = true;

				NvApiEnumGpus();
				NvApiEnumTccGpus();
			}
		}

		if (AdlLoad() == 0)
		{
			if (AdlInit() == 0)
			{
				adl = true;

				AdlEnumGpus();
			}
		}

		if ((nvapi) || (adl))
			ParseArgs(argc, argv);

		if (nvapi)
			NvApiFree();

		if (adl)
			AdlFree();
	}
	else
	{
		PrintUsage();
	}

	if (LogFile)
		fclose(LogFile);

	return EXIT_SUCCESS;
}