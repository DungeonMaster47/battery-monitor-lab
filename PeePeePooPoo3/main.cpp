#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <powrprof.h>
#pragma comment(lib, "powrprof.lib")

#define STRING_SIZE 512


int main()
{
	system("chcp 1251 > NUL");

	WCHAR oldSchemeName[STRING_SIZE] = { };
	SYSTEM_POWER_STATUS oldSPS = { };
	while (true)
	{
		if (_kbhit())
		{
			switch (_getch())
			{
			case '1':
				SetSuspendState(FALSE, NULL, FALSE);
				break;
			default:
				exit(0);
			}
		}

		SYSTEM_POWER_STATUS SPS;
		GetSystemPowerStatus(&SPS);

		WCHAR schemeName[STRING_SIZE];
		DWORD size = STRING_SIZE;
		GUID* scheme;
		PowerGetActiveScheme(NULL, &scheme);
		PowerReadFriendlyName(NULL, scheme, &NO_SUBGROUP_GUID, NULL, (PUCHAR)schemeName, &size);

		if ((SPS.ACLineStatus == oldSPS.ACLineStatus) &&
			(SPS.BatteryFlag == oldSPS.BatteryFlag) &&
			(SPS.BatteryLifePercent == oldSPS.BatteryLifePercent) &&
			(SPS.BatteryLifeTime == oldSPS.BatteryLifeTime) &&
			!lstrcmpW(schemeName, oldSchemeName))
			continue;

		lstrcpyW(oldSchemeName, schemeName);
		oldSPS = SPS;
		system("cls");

		char schemeNameA[STRING_SIZE];
		WideCharToMultiByte(1251, NULL, schemeName, STRING_SIZE, schemeNameA, STRING_SIZE, NULL, NULL);
		printf("Current power saving mode: %s\n", schemeNameA);

		switch (SPS.ACLineStatus)
		{
		case 0:
			printf("Working from battery\n");
			break;
		case 1:
			printf("Working from external source\n");
			break;
		default:
			printf("Unknown status of AC line\n");
			break;
		}

		switch (SPS.BatteryFlag)
		{
		case 128:
			printf("No battery\n");
			break;
		case 255:
			printf("Unable to get battery status\n");
			break;
		default:
			printf("Battery: %u%%	", SPS.BatteryLifePercent);
			if (SPS.ACLineStatus != 1 || SPS.BatteryLifeTime != -1)
				printf("Remaining time: %d hours %d minutes", (SPS.BatteryLifeTime / 60) / 60, (SPS.BatteryLifeTime / 60) % 60);
			break;
		}

		printf("\n1.Sleep\n");
	}
}