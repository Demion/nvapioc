#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_WARNINGS

#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAKE_NVAPI_VERSION(type, version) ((unsigned int)(sizeof(type) | ((version) << 16)))

#pragma pack(push, 1)

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
	unsigned int gpuHandle;
	unsigned int attribute;
	unsigned int value;
};

#pragma pack(pop)

void* (__cdecl *NvAPI_QueryInterface)(unsigned int offset) = 0;
int (__cdecl *NvAPI_Initialize)() = 0;
int (__cdecl *NvAPI_Unload)() = 0;
int (__cdecl *NvAPI_RestartDisplayDriver)() = 0;
int (__cdecl *NvAPI_EnumPhysicalGPUs)(unsigned int *gpuHandles, unsigned int *gpuCount) = 0;
int (__cdecl *NvAPI_EnumTCCPhysicalGPUs)(unsigned int *gpuHandles, unsigned int *gpuCount) = 0;
int (__cdecl *NvAPI_GPU_SetPstates20)(unsigned int gpuHandle, NV_GPU_PERF_PSTATES20_INFO *pStatesInfo) = 0;
int (__cdecl *NvAPI_GPU_GetClockBoostLock)(unsigned int gpuHandle, NV_GPU_CLOCK_LOCK *clockLock) = 0;
int (__cdecl *NvAPI_GPU_SetClockBoostLock)(unsigned int gpuHandle, NV_GPU_CLOCK_LOCK *clockLock) = 0;
int (__cdecl *NvAPI_GPU_ClientPowerPoliciesSetStatus)(unsigned int gpuHandle, NV_GPU_POWER_STATUS *powerStatus) = 0;
int (__cdecl *NvAPI_GPU_ClientThermalPoliciesSetLimit)(unsigned int gpuHandle, NV_GPU_THERMAL_LIMIT *thermalLimit) = 0;
int (__cdecl *NvAPI_GPU_SetCoolerLevels)(unsigned int gpuHandle, unsigned int coolerIndex, NV_GPU_COOLER_LEVEL *coolerLevel) = 0;
int (__cdecl *NvAPI_GPU_SetIllumination)(NV_GPU_SET_ILLUMINATION_PARM *illuminationInfo) = 0;
int (__cdecl *NvAPI_GPU_SetPstate)(unsigned int gpuHandle, unsigned int pState, unsigned int setType) = 0;
int (__cdecl *NvAPI_GPU_SetPstateClientLimits)(unsigned int gpuHandle, unsigned int limitType, unsigned int pState) = 0;
int (__cdecl *NvAPI_GPU_EnableDynamicPstates)(unsigned int gpuHandle, unsigned int dynamicPStates) = 0;
int (__cdecl *NvAPI_GPU_EnableOverclockedPstates)(unsigned int gpuHandle, unsigned int overclockedPStates) = 0;

enum ClockType
{
	Core = 0,
	Memory = 4
};

enum CoolerMode
{
	Manual = 1,
	Auto = 32
};

enum IlluminationType
{
	Logo = 0,
	SliBridge = 1
};

enum PStateLimitType
{
	Soft = 1,
	Hard = 2,
	All = 3
};

unsigned int GpuHandles[128] = {0};
unsigned int GpuCount = 0;

#define LOG(expression) Log(#expression, strrchr(__FILE__, '\\') + 1, __LINE__, (int) (expression));

FILE *LogFile = 0;

int Log(const char *expression, const char *fileName, int line, int result)
{
	if (LogFile)
	{
		time_t t = time(0);

		tm *local = localtime(&t);

		fprintf(LogFile, "[%02d.%02d.%04d %02d:%02d:%02d][%s:%d] %s = %d (0x%08X)\n",
				local->tm_mday, local->tm_mon + 1, local->tm_year + 1900, local->tm_hour, local->tm_min, local->tm_sec, fileName, line, expression, result, (unsigned int) result);

		fflush(LogFile);
	}

	return result;
}

int Load()
{
	int result = -1;

	HMODULE nvapi = 0;

	LOG(nvapi = LoadLibraryA("nvapi.dll"));

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
			NvAPI_EnumPhysicalGPUs = (int (__cdecl*)(unsigned int*, unsigned int*)) NvAPI_QueryInterface(0xE5AC921F);
			NvAPI_EnumTCCPhysicalGPUs = (int(__cdecl*)(unsigned int*, unsigned int*)) NvAPI_QueryInterface(0xD9930B07);
			NvAPI_GPU_SetPstates20 = (int (__cdecl*)(unsigned int, NV_GPU_PERF_PSTATES20_INFO*)) NvAPI_QueryInterface(0x0F4DAE6B);
			NvAPI_GPU_GetClockBoostLock = (int (__cdecl*)(unsigned int, NV_GPU_CLOCK_LOCK*)) NvAPI_QueryInterface(0xE440B867);
			NvAPI_GPU_SetClockBoostLock = (int (__cdecl*)(unsigned int, NV_GPU_CLOCK_LOCK*)) NvAPI_QueryInterface(0x39442CFB);
			NvAPI_GPU_ClientPowerPoliciesSetStatus = (int (__cdecl*)(unsigned int, NV_GPU_POWER_STATUS*)) NvAPI_QueryInterface(0xAD95F5ED);
			NvAPI_GPU_ClientThermalPoliciesSetLimit = (int (__cdecl*)(unsigned int, NV_GPU_THERMAL_LIMIT*)) NvAPI_QueryInterface(0x34C0B13D);
			NvAPI_GPU_SetCoolerLevels = (int (__cdecl*)(unsigned int, unsigned int, NV_GPU_COOLER_LEVEL*)) NvAPI_QueryInterface(0x891FA0AE);
			NvAPI_GPU_SetIllumination = (int (__cdecl*)(NV_GPU_SET_ILLUMINATION_PARM*)) NvAPI_QueryInterface(0x0254A187);
			NvAPI_GPU_SetPstate = (int (__cdecl*)(unsigned int, unsigned int, unsigned int)) NvAPI_QueryInterface(0x025BFB10);
			NvAPI_GPU_SetPstateClientLimits = (int (__cdecl*)(unsigned int, unsigned int, unsigned int)) NvAPI_QueryInterface(0xFDFC7D49);
			NvAPI_GPU_EnableDynamicPstates = (int (__cdecl*)(unsigned int, unsigned int)) NvAPI_QueryInterface(0xFA579A0F);
			NvAPI_GPU_EnableOverclockedPstates = (int (__cdecl*)(unsigned int, unsigned int)) NvAPI_QueryInterface(0xB23B70EE);
		}
	}

	return result;
}

int Init()
{
	int result = -1;

	if (NvAPI_Initialize)
	{
		LOG(result = NvAPI_Initialize());
	}

	return result;
}

int Free()
{
	int result = -1;

	if (NvAPI_Unload)
	{
		LOG(result = NvAPI_Unload());
	}

	return result;
}

int RestartDriver()
{
	int result = -1;

	if (NvAPI_RestartDisplayDriver)
	{
		LOG(result = NvAPI_RestartDisplayDriver());
	}

	return result;
}

int EnumGpus()
{
	int result = -1;

	if (NvAPI_EnumPhysicalGPUs)
	{
		unsigned int count = 0;

		LOG(result = NvAPI_EnumPhysicalGPUs(GpuHandles + GpuCount, &count));

		GpuCount += count;
	}

	return result;
}

int EnumTccGpus()
{
	int result = -1;

	if (NvAPI_EnumTCCPhysicalGPUs)
	{
		unsigned int count = 0;

		LOG(result = NvAPI_EnumTCCPhysicalGPUs(GpuHandles + GpuCount, &count));

		GpuCount += count;
	}

	return result;
}

int SetClockOffset(unsigned int gpuIndex, unsigned int pState, ClockType clockType, int frequencyOffsetKHz)
{
	int result = -1;

	if (NvAPI_GPU_SetPstates20)
	{
		if (gpuIndex < GpuCount)
		{
			NV_GPU_PERF_PSTATES20_INFO pStatesInfo = {0};

			pStatesInfo.version = MAKE_NVAPI_VERSION(pStatesInfo, 2);
			pStatesInfo.numPStates = 1;
			pStatesInfo.numClocks = 1;
			pStatesInfo.pStates[0].pStateId = pState;
			pStatesInfo.pStates[0].clocks[0].domainId = clockType;
			pStatesInfo.pStates[0].clocks[0].typeId = 0;
			pStatesInfo.pStates[0].clocks[0].frequencyDeltaKHz.value = frequencyOffsetKHz;

			LOG(result = NvAPI_GPU_SetPstates20(GpuHandles[gpuIndex], &pStatesInfo));
		}
	}

	return result;
}

int SetVoltageOffset(unsigned int gpuIndex, unsigned int pState, int voltageOffsetUV)
{
	int result = -1;

	if (NvAPI_GPU_SetPstates20)
	{
		if (gpuIndex < GpuCount)
		{
			NV_GPU_PERF_PSTATES20_INFO pStatesInfo = {0};

			pStatesInfo.version = MAKE_NVAPI_VERSION(pStatesInfo, 2);
			pStatesInfo.numPStates = 1;
			pStatesInfo.numBaseVoltages = 1;
			pStatesInfo.pStates[0].pStateId = pState;
			pStatesInfo.pStates[0].baseVoltages[0].domainId = 0;
			pStatesInfo.pStates[0].baseVoltages[0].voltageDeltaUV.value = voltageOffsetUV;

			LOG(result = NvAPI_GPU_SetPstates20(GpuHandles[gpuIndex], &pStatesInfo));
		}
	}

	return result;
}

int SetOverVoltageOffset(unsigned int gpuIndex, int voltageOffsetUV)
{
	int result = -1;

	if (NvAPI_GPU_SetPstates20)
	{
		if (gpuIndex < GpuCount)
		{
			NV_GPU_PERF_PSTATES20_INFO pStatesInfo = {0};

			pStatesInfo.version = MAKE_NVAPI_VERSION(pStatesInfo, 2);
			pStatesInfo.overVoltage.numVoltages = 1;
			pStatesInfo.overVoltage.voltages[0].domainId = 0;
			pStatesInfo.overVoltage.voltages[0].voltageDeltaUV.value = voltageOffsetUV;

			LOG(result = NvAPI_GPU_SetPstates20(GpuHandles[gpuIndex], &pStatesInfo));
		}
	}

	return result;
}

int SetVoltageLock(unsigned int gpuIndex, unsigned int voltageUV)
{
	int result = -1;

	if ((NvAPI_GPU_GetClockBoostLock) && (NvAPI_GPU_SetClockBoostLock))
	{
		if (gpuIndex < GpuCount)
		{
			NV_GPU_CLOCK_LOCK clockLock = {0};

			clockLock.version = MAKE_NVAPI_VERSION(clockLock, 1);

			LOG(result = NvAPI_GPU_GetClockBoostLock(GpuHandles[gpuIndex], &clockLock));

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

				LOG(result = NvAPI_GPU_SetClockBoostLock(GpuHandles[gpuIndex], &clockLock));
			}
		}
	}

	return result;
}

int SetPowerLimit(unsigned int gpuIndex, unsigned int power)
{
	int result = -1;

	if (NvAPI_GPU_ClientPowerPoliciesSetStatus)
	{
		if (gpuIndex < GpuCount)
		{
			NV_GPU_POWER_STATUS powerStatus = {0};

			powerStatus.version = MAKE_NVAPI_VERSION(powerStatus, 1);
			powerStatus.count = 1;
			powerStatus.entries[0].unknown1 = 0;
			powerStatus.entries[0].power = power * 1000;

			LOG(result = NvAPI_GPU_ClientPowerPoliciesSetStatus(GpuHandles[gpuIndex], &powerStatus));
		}
	}

	return result;
}

int SetTempLimit(unsigned int gpuIndex, bool priority, unsigned int tempC)
{
	int result = -1;

	if (NvAPI_GPU_ClientThermalPoliciesSetLimit)
	{
		if (gpuIndex < GpuCount)
		{
			NV_GPU_THERMAL_LIMIT thermalLimit = {0};

			thermalLimit.version = MAKE_NVAPI_VERSION(thermalLimit, 2);
			thermalLimit.count = 1;
			thermalLimit.entries[0].controller = 1;
			thermalLimit.entries[0].value = tempC << 8;
			thermalLimit.entries[0].flags = priority ? 1 : 0;

			LOG(result = NvAPI_GPU_ClientThermalPoliciesSetLimit(GpuHandles[gpuIndex], &thermalLimit));
		}
	}

	return result;
}

int SetCoolerLevel(unsigned int gpuIndex, unsigned int coolerIndex, CoolerMode coolerMode, unsigned int level)
{
	int result = -1;

	if (NvAPI_GPU_SetCoolerLevels)
	{
		if (gpuIndex < GpuCount)
		{
			NV_GPU_COOLER_LEVEL coolerLevel = {0};

			coolerLevel.version = MAKE_NVAPI_VERSION(coolerLevel, 1);
			coolerLevel.coolers[0].level = level;
			coolerLevel.coolers[0].policy = coolerMode;

			LOG(result = NvAPI_GPU_SetCoolerLevels(GpuHandles[gpuIndex], coolerIndex, &coolerLevel));
		}
	}

	return result;
}

int SetIllumination(unsigned int gpuIndex, IlluminationType illuminationType, unsigned int brightness)
{
	int result = -1;

	if (NvAPI_GPU_SetIllumination)
	{
		if (gpuIndex < GpuCount)
		{
			NV_GPU_SET_ILLUMINATION_PARM illuminationInfo = {0};

			illuminationInfo.version = MAKE_NVAPI_VERSION(illuminationInfo, 1);
			illuminationInfo.gpuHandle = GpuHandles[gpuIndex];
			illuminationInfo.attribute = illuminationType;
			illuminationInfo.value = brightness;

			LOG(result = NvAPI_GPU_SetIllumination(&illuminationInfo));
		}
	}

	return result;
}

int SetPState(unsigned int gpuIndex, unsigned int pState)
{
	int result = -1;

	if (NvAPI_GPU_SetPstate)
	{
		if (gpuIndex < GpuCount)
		{
			LOG(result = NvAPI_GPU_SetPstate(GpuHandles[gpuIndex], pState, 2));
		}
	}

	return result;
}

int SetPStateLimit(unsigned int gpuIndex, PStateLimitType limitType, unsigned int pState)
{
	int result = -1;

	if (NvAPI_GPU_SetPstateClientLimits)
	{
		if (gpuIndex < GpuCount)
		{
			LOG(result = NvAPI_GPU_SetPstateClientLimits(GpuHandles[gpuIndex], limitType, pState));
		}
	}

	return result;
}

void PrintUsage()
{
	printf(
		"\n"
		"Usage:\n"
		"-core gpuIndex pState frequencyOffsetKHz (0 = default)\n"
		"-mem gpuIndex pState frequencyOffsetKHz (0 = default)\n"
		"-volt gpuIndex voltageUV (0 = default)\n"
		"-power gpuIndex power\n"
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
				int frequencyOffsetKHz = atoi(argv[++arg]);

				SetClockOffset(gpuIndex, pState, ClockType::Core, frequencyOffsetKHz);
			}
		}
		else if (strcmp(strupr(argv[arg]), "-MEM") == 0)
		{
			if (arg + 3 < argc)
			{
				unsigned int gpuIndex = atoi(argv[++arg]);
				unsigned int pState = atoi(argv[++arg]);
				int frequencyOffsetKHz = atoi(argv[++arg]);

				SetClockOffset(gpuIndex, pState, ClockType::Memory, frequencyOffsetKHz);
			}
		}
		else if (strcmp(strupr(argv[arg]), "-VOLT") == 0)
		{
			if (arg + 2 < argc)
			{
				unsigned int gpuIndex = atoi(argv[++arg]);
				unsigned int voltageUV = atoi(argv[++arg]);

				SetVoltageLock(gpuIndex, voltageUV);
			}
		}
		else if (strcmp(strupr(argv[arg]), "-POWER") == 0)
		{
			if (arg + 2 < argc)
			{
				unsigned int gpuIndex = atoi(argv[++arg]);
				unsigned int power = atoi(argv[++arg]);

				SetPowerLimit(gpuIndex, power);
			}
		}
		else if (strcmp(strupr(argv[arg]), "-TEMP") == 0)
		{
			if (arg + 3 < argc)
			{
				unsigned int gpuIndex = atoi(argv[++arg]);
				unsigned int priority = atoi(argv[++arg]);
				unsigned int tempC = atoi(argv[++arg]);

				SetTempLimit(gpuIndex, (priority != 0), tempC);
			}
		}
		else if (strcmp(strupr(argv[arg]), "-FAN") == 0)
		{
			if (arg + 3 < argc)
			{
				unsigned int gpuIndex = atoi(argv[++arg]);
				unsigned int coolerIndex = atoi(argv[++arg]);
				int level = atoi(argv[++arg]);

				SetCoolerLevel(gpuIndex, coolerIndex, level >= 0 ? CoolerMode::Manual : CoolerMode::Auto, level >= 0 ? level : 30);
			}
		}
		else if (strcmp(strupr(argv[arg]), "-LED") == 0)
		{
			if (arg + 3 < argc)
			{
				unsigned int gpuIndex = atoi(argv[++arg]);
				unsigned int type = atoi(argv[++arg]);
				unsigned int brightness = atoi(argv[++arg]);

				SetIllumination(gpuIndex, (IlluminationType) type, brightness);
			}
		}
		else if (strcmp(strupr(argv[arg]), "-RESTART") == 0)
		{
			RestartDriver();
		}
		else
		{
			printf("Invalid parameter #%d \"%s\"\n", arg, argv[arg]);
		}

		++arg;
	}
}

int main(int argc, char *argv[])
{
	LogFile = fopen("log.txt", "a");

	if (argc > 1)
	{
		if (Load() == 0)
		{
			if (Init() == 0)
			{
				EnumGpus();
				EnumTccGpus();

				ParseArgs(argc, argv);

				Free();
			}
		}
	}
	else
	{
		PrintUsage();
	}

	if (LogFile)
		fclose(LogFile);

	return EXIT_SUCCESS;
}