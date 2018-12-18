#define _CRT_NONSTDC_NO_WARNINGS

#include <Windows.h>
#include <stdio.h>
#include <string.h>

#define MAKE_NVAPI_VERSION(typeName, ver) ((unsigned int)(sizeof(typeName) | ((ver) << 16)))

struct NV_GPU_PERF_PSTATES20_PARAM_DELTA
{
	int value;

	struct
	{
		int min;
		int max;
	} valueRange;
};

struct NV_GPU_PSTATE20_CLOCK_ENTRY
{
	unsigned int domainId;
	unsigned int typeId;
	unsigned int editable : 1;
	unsigned int reserved : 31;
	NV_GPU_PERF_PSTATES20_PARAM_DELTA freqDeltaKHz;

	union
	{
		struct
		{
			int freqKHz;
		} single;

		struct
		{
			int minFreqKHz;
			int maxFreqKHz;
			unsigned int domainId;
			int minVoltageUV;
			int maxVoltageUV;
		} range;
	} data;
};

struct NV_GPU_PSTATE20_BASE_VOLTAGE_ENTRY
{
	unsigned int domainId;
	unsigned int editable : 1;
	unsigned int reserved : 31;
	int voltUV;
	NV_GPU_PERF_PSTATES20_PARAM_DELTA voltDeltaUV;
};

struct NV_GPU_PERF_PSTATES20_INFO
{
	unsigned int version;
	unsigned int editable : 1;
	unsigned int reserved : 31;
	unsigned int numPstates;
	unsigned int numClocks;
	unsigned int numBaseVoltages;

	struct
	{
		unsigned int pstateId;
		unsigned int editable : 1;
		unsigned int reserved : 31;
		NV_GPU_PSTATE20_CLOCK_ENTRY clocks[8];
		NV_GPU_PSTATE20_BASE_VOLTAGE_ENTRY baseVoltages[4];
	} pstates[16];

	struct
	{
		unsigned int numVoltages;
		NV_GPU_PSTATE20_BASE_VOLTAGE_ENTRY voltages[4];
	} ov;
};

struct NV_GPU_CLOCK_LOCK_ENTRY
{
	unsigned int index;
	unsigned int unk1;
	unsigned int mode;
	unsigned int unk2;
	unsigned int voltageUV;
	unsigned int unk3;
};

struct NV_GPU_CLOCK_LOCK
{
	unsigned int version;
	unsigned int flags;
	unsigned int count;
	NV_GPU_CLOCK_LOCK_ENTRY entry[32];
};

struct NV_GPU_POWER_STATUS_ENTRY
{
	unsigned int unk1;
	unsigned int unk2;
	unsigned int power;
	unsigned int unk3;
};

struct NV_GPU_POWER_STATUS
{
	unsigned int version;
	unsigned int count;
	NV_GPU_POWER_STATUS_ENTRY entry[4];
};

struct NV_GPU_THERMAL_LIMIT_ENTRY
{
	unsigned int controller;
	unsigned int value;
	unsigned int flags;
};

struct NV_GPU_THERMAL_LIMIT
{
	unsigned int version;
	unsigned int count;
	NV_GPU_THERMAL_LIMIT_ENTRY entry[4];
};

struct NV_GPU_SETCOOLER_LEVEL_COOLER
{
	unsigned int currentLevel;
	unsigned int currentPolicy;
};

struct NV_GPU_SETCOOLER_LEVEL
{
	unsigned int version;
	NV_GPU_SETCOOLER_LEVEL_COOLER cooler[3];
};

struct NV_GPU_GET_ILLUMINATION_PARM
{
	unsigned int version;
	unsigned int gpuHandle;
	unsigned int attribute;
	unsigned int value;
};

typedef NV_GPU_GET_ILLUMINATION_PARM NV_GPU_SET_ILLUMINATION_PARM;

void* (__cdecl *NvAPI_QueryInterface)(unsigned int id) = 0;
int (__cdecl *NvAPI_Initialize)() = 0;
int (__cdecl *NvAPI_Unload)() = 0;
int (__cdecl *NvAPI_EnumPhysicalGPUs)(unsigned int **gpuHandle, unsigned int *gpuCount) = 0;
int (__cdecl *NvAPI_GPU_GetPstates20)(unsigned int gpuHandle, NV_GPU_PERF_PSTATES20_INFO *pStatesInfo) = 0;
int (__cdecl *NvAPI_GPU_SetPstates20)(unsigned int gpuHandle, NV_GPU_PERF_PSTATES20_INFO *pStatesInfo) = 0;
int (__cdecl *NvAPI_GPU_GetClockBoostLock)(unsigned int gpuHandle, NV_GPU_CLOCK_LOCK *clockLock) = 0;
int (__cdecl *NvAPI_GPU_SetClockBoostLock)(unsigned int gpuHandle, NV_GPU_CLOCK_LOCK *clockLock) = 0;
int (__cdecl *NvAPI_GPU_ClientPowerPoliciesGetStatus)(unsigned int gpuHandle, NV_GPU_POWER_STATUS *powerStatus) = 0;
int (__cdecl *NvAPI_GPU_ClientPowerPoliciesSetStatus)(unsigned int gpuHandle, NV_GPU_POWER_STATUS *powerStatus) = 0;
int (__cdecl *NvAPI_GPU_ClientThermalPoliciesGetLimit)(unsigned int gpuHandle, NV_GPU_THERMAL_LIMIT *thermalLimit) = 0;
int (__cdecl *NvAPI_GPU_ClientThermalPoliciesSetLimit)(unsigned int gpuHandle, NV_GPU_THERMAL_LIMIT *thermalLimit) = 0;
int (__cdecl *NvAPI_GPU_SetCoolerLevels)(unsigned int gpuHandle, unsigned int coolerIndex, NV_GPU_SETCOOLER_LEVEL *coolerLevel) = 0;
int (__cdecl *NvAPI_GPU_GetIllumination)(NV_GPU_GET_ILLUMINATION_PARM *illuminationInfo) = 0;
int (__cdecl *NvAPI_GPU_SetIllumination)(NV_GPU_SET_ILLUMINATION_PARM *illuminationInfo) = 0;
int (__cdecl *NvAPI_GPU_SetPstate)(unsigned int gpuHandle, unsigned int pState) = 0;

unsigned int GpuHandle[64] = {0};
unsigned int GpuCount = 0;

bool Load()
{
	HMODULE nvapi = LoadLibraryA("nvapi.dll");

	if (nvapi)
	{
		NvAPI_QueryInterface = (void* (__cdecl*)(unsigned int)) GetProcAddress(nvapi, "nvapi_QueryInterface");

		if (NvAPI_QueryInterface)
		{
			NvAPI_Initialize = (int (__cdecl *)()) NvAPI_QueryInterface(0x0150E828);
			NvAPI_Unload = (int (__cdecl*)()) NvAPI_QueryInterface(0xD22BDD7E);
			NvAPI_EnumPhysicalGPUs = (int (__cdecl*)(unsigned int**, unsigned int*)) NvAPI_QueryInterface(0xE5AC921F);
			NvAPI_GPU_GetPstates20 = (int (__cdecl*)(unsigned int, NV_GPU_PERF_PSTATES20_INFO*)) NvAPI_QueryInterface(0x6FF81213);
			NvAPI_GPU_SetPstates20 = (int (__cdecl*)(unsigned int, NV_GPU_PERF_PSTATES20_INFO*)) NvAPI_QueryInterface(0x0F4DAE6B);
			NvAPI_GPU_GetClockBoostLock = (int (__cdecl*)(unsigned int, NV_GPU_CLOCK_LOCK*)) NvAPI_QueryInterface(0xE440B867);
			NvAPI_GPU_SetClockBoostLock = (int (__cdecl*)(unsigned int, NV_GPU_CLOCK_LOCK*)) NvAPI_QueryInterface(0x39442CFB);
			NvAPI_GPU_ClientPowerPoliciesGetStatus = (int (__cdecl*)(unsigned int, NV_GPU_POWER_STATUS*)) NvAPI_QueryInterface(0x70916171);
			NvAPI_GPU_ClientPowerPoliciesSetStatus = (int (__cdecl*)(unsigned int, NV_GPU_POWER_STATUS*)) NvAPI_QueryInterface(0xAD95F5ED);
			NvAPI_GPU_ClientThermalPoliciesGetLimit = (int (__cdecl*)(unsigned int, NV_GPU_THERMAL_LIMIT*)) NvAPI_QueryInterface(0xE9C425A1);
			NvAPI_GPU_ClientThermalPoliciesSetLimit = (int (__cdecl*)(unsigned int, NV_GPU_THERMAL_LIMIT*)) NvAPI_QueryInterface(0x34C0B13D);
			NvAPI_GPU_SetCoolerLevels = (int (__cdecl*)(unsigned int, unsigned int, NV_GPU_SETCOOLER_LEVEL*)) NvAPI_QueryInterface(0x891FA0AE);
			NvAPI_GPU_GetIllumination = (int (__cdecl*)(NV_GPU_GET_ILLUMINATION_PARM*)) NvAPI_QueryInterface(0x9A1B9365);
			NvAPI_GPU_SetIllumination = (int (__cdecl*)(NV_GPU_GET_ILLUMINATION_PARM*)) NvAPI_QueryInterface(0x0254A187);
			NvAPI_GPU_SetPstate = (int (__cdecl*)(unsigned int, unsigned int)) NvAPI_QueryInterface(0x025BFB10);

			return true;
		}
	}

	return false;
}

int Init()
{
	if (NvAPI_Initialize)
	{
		return NvAPI_Initialize();
	}

	return -1;
}

int Free()
{
	if (NvAPI_Unload)
	{
		return NvAPI_Unload();
	}

	return -1;
}

int EnumGPU()
{
	if (NvAPI_EnumPhysicalGPUs)
	{
		return NvAPI_EnumPhysicalGPUs((unsigned int**) &GpuHandle, &GpuCount);
	}

	return -1;
}

int SetClock(unsigned int gpuIndex, unsigned int pState, int offsetKHz, unsigned int type)
{
	if ((NvAPI_GPU_SetPstates20) && (gpuIndex < GpuCount))
	{
		NV_GPU_PERF_PSTATES20_INFO pStatesInfo = {0};

		pStatesInfo.version = MAKE_NVAPI_VERSION(pStatesInfo, 2);
		pStatesInfo.numPstates = 1;
		pStatesInfo.numClocks = 1;
		pStatesInfo.pstates[0].pstateId = pState;
		pStatesInfo.pstates[0].clocks[0].domainId = type;
		pStatesInfo.pstates[0].clocks[0].typeId = 0;
		pStatesInfo.pstates[0].clocks[0].freqDeltaKHz.value = offsetKHz;

		return NvAPI_GPU_SetPstates20(GpuHandle[gpuIndex], &pStatesInfo);
	}

	return -1;
}

int SetCoreClock(unsigned int gpuIndex, unsigned int pState, int offsetKHz)
{
	return SetClock(gpuIndex, pState, offsetKHz, 0);
}

int SetMemoryClock(unsigned int gpuIndex, unsigned int pState, int offsetKHz)
{
	return SetClock(gpuIndex, pState, offsetKHz, 4);
}

int SetVoltage(unsigned int gpuIndex, unsigned int pState, int offsetUV)
{
	if ((NvAPI_GPU_SetPstates20) && (gpuIndex < GpuCount))
	{
		NV_GPU_PERF_PSTATES20_INFO pStatesInfo = {0};

		pStatesInfo.version = MAKE_NVAPI_VERSION(pStatesInfo, 2);
		pStatesInfo.numPstates = 1;
		pStatesInfo.numBaseVoltages = 1;
		pStatesInfo.pstates[0].pstateId = pState;
		pStatesInfo.pstates[0].baseVoltages[0].domainId = 0;
		pStatesInfo.pstates[0].baseVoltages[0].voltDeltaUV.value = offsetUV;

		return NvAPI_GPU_SetPstates20(GpuHandle[gpuIndex], &pStatesInfo);
	}

	return -1;
}

int SetOverVoltage(unsigned int gpuIndex, int offsetUV)
{
	if ((NvAPI_GPU_SetPstates20) && (gpuIndex < GpuCount))
	{
		NV_GPU_PERF_PSTATES20_INFO pStatesInfo = {0};

		pStatesInfo.version = MAKE_NVAPI_VERSION(pStatesInfo, 2);
		pStatesInfo.ov.numVoltages = 1;
		pStatesInfo.ov.voltages[0].domainId = 0;
		pStatesInfo.ov.voltages[0].voltDeltaUV.value = offsetUV;

		return NvAPI_GPU_SetPstates20(GpuHandle[gpuIndex], &pStatesInfo);
	}

	return -1;
}

int SetVoltageLock(unsigned int gpuIndex, unsigned int voltageUV)
{
	if ((NvAPI_GPU_GetClockBoostLock) && (NvAPI_GPU_SetClockBoostLock) && (gpuIndex < GpuCount))
	{
		NV_GPU_CLOCK_LOCK clockLock = {0};

		clockLock.version = MAKE_NVAPI_VERSION(clockLock, 1);

		NvAPI_GPU_GetClockBoostLock(GpuHandle[gpuIndex], &clockLock);

		for (unsigned int i = 0; i < clockLock.count; ++i)
		{
			if (clockLock.entry[i].index == 6)
			{
				clockLock.entry[i].mode = (voltageUV != 0) ? 3 : 0;
				clockLock.entry[i].voltageUV = voltageUV;
				clockLock.entry[i].unk3 = 0;
			}
		}

		return NvAPI_GPU_SetClockBoostLock(GpuHandle[gpuIndex], &clockLock);
	}

	return -1;
}

int SetPower(unsigned int gpuIndex, unsigned int power)
{
	if ((NvAPI_GPU_ClientPowerPoliciesSetStatus) && (gpuIndex < GpuCount))
	{
		NV_GPU_POWER_STATUS powerStatus = {0};

		powerStatus.version = MAKE_NVAPI_VERSION(powerStatus, 1);
		powerStatus.count = 1;
		powerStatus.entry[0].unk1 = 0;
		powerStatus.entry[0].power = power * 1000;

		return NvAPI_GPU_ClientPowerPoliciesSetStatus(GpuHandle[gpuIndex], &powerStatus);
	}

	return -1;
}

int SetTempLimit(unsigned int gpuIndex, unsigned int tempC, bool priority)
{
	if ((NvAPI_GPU_ClientThermalPoliciesSetLimit) && (gpuIndex < GpuCount))
	{
		NV_GPU_THERMAL_LIMIT thermalLimit = {0};

		thermalLimit.version = MAKE_NVAPI_VERSION(thermalLimit, 2);
		thermalLimit.count = 1;
		thermalLimit.entry[0].controller = 1;
		thermalLimit.entry[0].value = tempC << 8;
		thermalLimit.entry[0].flags = priority ? 1 : 0;

		return NvAPI_GPU_ClientThermalPoliciesSetLimit(GpuHandle[gpuIndex], &thermalLimit);
	}

	return -1;
}

int SetCoolerLevel(unsigned int gpuIndex, unsigned int coolerIndex, int level)
{
	if ((NvAPI_GPU_SetCoolerLevels) && (gpuIndex < GpuCount))
	{
		NV_GPU_SETCOOLER_LEVEL coolerLevel = {0};

		coolerLevel.version = MAKE_NVAPI_VERSION(coolerLevel, 1);
		coolerLevel.cooler[0].currentLevel = (level >= 0) ? level : 30;
		coolerLevel.cooler[0].currentPolicy = (level > 0) ? 1 : 32;

		return NvAPI_GPU_SetCoolerLevels(GpuHandle[gpuIndex], coolerIndex, &coolerLevel);
	}

	return -1;
}

int SetIllumination(unsigned int gpuIndex, unsigned int type, unsigned int brightness)
{
	if ((NvAPI_GPU_SetIllumination) && (gpuIndex < GpuCount))
	{
		NV_GPU_GET_ILLUMINATION_PARM illuminationInfo = {0};

		illuminationInfo.version = MAKE_NVAPI_VERSION(illuminationInfo, 1);
		illuminationInfo.gpuHandle = GpuHandle[gpuIndex];
		illuminationInfo.attribute = type;
		illuminationInfo.value = brightness;

		return NvAPI_GPU_SetIllumination(&illuminationInfo);
	}

	return -1;
}

int SetPState(unsigned int gpuIndex, unsigned int pState)
{
	if ((NvAPI_GPU_SetPstate) && (gpuIndex < GpuCount))
	{
		return NvAPI_GPU_SetPstate(GpuHandle[gpuIndex], pState);
	}

	return -1;
}

void PrintUsage()
{
	printf(
		"Usage:\n"
		"-core gpuIndex pState offsetKHz (0 = default)\n"
		"-mem gpuIndex pState offsetKHz (0 = default)\n"
		"-volt gpuIndex voltageUV (0 = default)\n"
		"-power gpuIndex power\n"
		"-temp gpuIndex tempC priority (0 = false 1 = true)\n"
		"-fan gpuIndex fanIndex level (-1 = auto)\n"
		"-led gpuIndex type brightness (0 = logo 1 = slibridge)\n"
		"-pstate gpuIndex pState (16 = auto)\n");
}

void ParseArg(int argc, char *argv[])
{
	int arg = 1;

	while (arg < argc)
	{
		if ((strcmp(strupr(argv[arg]), "-CORE") == 0) && (arg + 3 < argc))
		{
			unsigned int gpuIndex = atoi(argv[++arg]);
			unsigned int pState = atoi(argv[++arg]);
			int offsetKHz = atoi(argv[++arg]);

			int result = SetCoreClock(gpuIndex, pState, offsetKHz);

			printf("SetCoreClock(%d, %d, %d) = %d\n", gpuIndex, pState, offsetKHz, result);
		}
		else if ((strcmp(strupr(argv[arg]), "-MEM") == 0) && (arg + 3 < argc))
		{
			unsigned int gpuIndex = atoi(argv[++arg]);
			unsigned int pState = atoi(argv[++arg]);
			int offsetKHz = atoi(argv[++arg]);

			int result = SetMemoryClock(gpuIndex, pState, offsetKHz);

			printf("SetMemoryClock(%d, %d, %d) = %d\n", gpuIndex, pState, offsetKHz, result);
		}
		else if ((strcmp(strupr(argv[arg]), "-VOLT") == 0) && (arg + 2 < argc))
		{
			unsigned int gpuIndex = atoi(argv[++arg]);
			unsigned int voltageUV = atoi(argv[++arg]);

			int result = SetVoltageLock(gpuIndex, voltageUV);

			printf("SetVoltageLock(%d, %d) = %d\n", gpuIndex, voltageUV, result);
		}
		else if ((strcmp(strupr(argv[arg]), "-POWER") == 0) && (arg + 2 < argc))
		{
			unsigned int gpuIndex = atoi(argv[++arg]);
			unsigned int power = atoi(argv[++arg]);

			int result = SetPower(gpuIndex, power);

			printf("SetPower(%d, %d) = %d\n", gpuIndex, power, result);
		}
		else if ((strcmp(strupr(argv[arg]), "-TEMP") == 0) && (arg + 3 < argc))
		{
			unsigned int gpuIndex = atoi(argv[++arg]);
			unsigned int tempC = atoi(argv[++arg]);
			unsigned int priority = atoi(argv[++arg]);

			int result = SetTempLimit(gpuIndex, tempC, (priority != 0));

			printf("SetTempLimit(%d, %d, %d) = %d\n", gpuIndex, tempC, priority, result);
		}
		else if ((strcmp(strupr(argv[arg]), "-FAN") == 0) && (arg + 3 < argc))
		{
			unsigned int gpuIndex = atoi(argv[++arg]);
			unsigned int coolerIndex = atoi(argv[++arg]);
			int level = atoi(argv[++arg]);

			int result = SetCoolerLevel(gpuIndex, coolerIndex, level);

			printf("SetCoolerLevel(%d, %d, %d) = %d\n", gpuIndex, coolerIndex, level, result);
		}
		else if ((strcmp(strupr(argv[arg]), "-LED") == 0) && (arg + 3 < argc))
		{
			unsigned int gpuIndex = atoi(argv[++arg]);
			unsigned int type = atoi(argv[++arg]);
			unsigned int brightness = atoi(argv[++arg]);

			int result = SetIllumination(gpuIndex, type, brightness);

			printf("SetIllumination(%d, %d, %d) = %d\n", gpuIndex, type, brightness, result);
		}
		else if ((strcmp(strupr(argv[arg]), "-PSTATE") == 0) && (arg + 2 < argc))
		{
			unsigned int gpuIndex = atoi(argv[++arg]);
			unsigned int pState = atoi(argv[++arg]);

			int result = SetPState(gpuIndex, pState);

			printf("SetPState(%d, %d) = %d\n", gpuIndex, pState, result);
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
	printf("\nNvApiOc v0.1\n\n");

	if (argc > 1)
	{
		int result = 0;

		result = Load();

		printf("Load() = %d\n", result);

		result = Init();

		printf("Init() = %d\n", result);

		result = EnumGPU();

		printf("EnumGPU() = %d\n", result);

		ParseArg(argc, argv);

		result = Free();

		printf("Free() = %d\n", result);
	}
	else
	{
		PrintUsage();
	}

	return EXIT_SUCCESS;
}