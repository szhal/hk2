#include <windows.h>
#include "atsplugin.h"
#include "hk.hpp"
#include "Ats.h"

#ifdef __cplusplus
extern "C" {
#endif

BOOL APIENTRY DllMain( HINSTANCE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved )
{
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			g_hk.BrakeNotch = &g_brakeNotch;
			g_hk.Reverser = &g_reverser;
			g_hk.Time = &g_time;
			g_hk.TrainSpeed = &g_speed;
			g_hk.DeltaT = &g_deltaT;
			break;

		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
	}

	return TRUE;
}

#ifdef __cplusplus
};
#endif

ATS_API void WINAPI Load()
{
}

ATS_API void WINAPI Dispose()
{
}

ATS_API int WINAPI GetPluginVersion()
{
	return ATS_VERSION;
}

ATS_API void WINAPI SetVehicleSpec(ATS_VEHICLESPEC vehicleSpec)
{
	g_svcBrake = vehicleSpec.BrakeNotches;
	g_emgBrake = g_svcBrake + 1;

	g_hk.EmgBrake = g_emgBrake;
	g_hk.SvcBrake = vehicleSpec.AtsNotch;
}

ATS_API void WINAPI Initialize(int brake)
{
	g_hk.initialize();
	g_speed = 0;
}

ATS_API ATS_HANDLES WINAPI Elapse(ATS_VEHICLESTATE vehicleState, int *panel, int *sound)
{
	g_deltaT = vehicleState.Time - g_time;
	g_time = vehicleState.Time;
	g_speed = vehicleState.Speed;

	g_hk.execute();

	// ハンドル出力
	if(g_hk.AtsBrake)
	{
		g_output.Brake = g_emgBrake;
	}
	else
	{
		g_output.Brake = g_brakeNotch;
	}
	if(g_pilotlamp)
	{
		g_output.Reverser = g_reverser;
	}
	else
	{
		g_output.Reverser = 0;
	}
	if(g_hk.Ats_N)
	{
		// 2012/08/14 信号Nのとき 力行できないようにした
		g_output.Power = 0;
	}
	else
	{
		g_output.Power = g_powerNotch;
	}
	if(g_output.Brake > 0)
	{
		g_output.ConstantSpeed = ATS_CONSTANTSPEED_DISABLE;
	}
	else
	{
		g_output.ConstantSpeed = ATS_CONSTANTSPEED_CONTINUE;
	}

	// パネル出力
	panel[0] = g_hk.Indicator;

	panel[10] = g_hk.Ats_0;
	panel[11] = g_hk.Ats_20;
	panel[12] = g_hk.Ats_30;
	panel[13] = g_hk.Ats_50;
	panel[14] = g_hk.Ats_70;
	panel[15] = g_hk.Ats_80;
	panel[16] = g_hk.Ats_F;

	panel[20] = g_hk.Ats_R20;
	panel[21] = g_hk.Ats_R30;
	panel[22] = g_hk.Ats_R50;
	panel[23] = g_hk.Ats_R70;
	panel[24] = g_hk.Ats_R80;
	panel[25] = g_hk.Ats_RF;
	panel[26] = g_hk.Ats_P;

	panel[30] = g_hk.Ats_N;
	panel[31] = g_hk.Ats_HP;
	panel[32] = g_hk.Ats_RN;

	panel[35] = g_hk.Ats_Confirm;
	panel[36] = g_hk.Replace;

	panel[50] = vehicleState.BcPressure < 100 ? 10 : vehicleState.BcPressure / 100; // Bc100の桁
	panel[51] = vehicleState.BcPressure < 10 ? 0 : vehicleState.BcPressure / 10 - (panel[50] % 10) * 10; // Bc10の桁

	// サウンド出力
	sound[0] = g_hk.BeginPattern; // パターン発生
	sound[1] = g_hk.ConfirmBuzz; // 確認モードブザー
	sound[2] = g_hk.SpeedOverBuzz; // 速度超過時ブザー
	sound[5] = g_hk.ReplaceSw; // 入換スイッチ

	return g_output;
}

ATS_API void WINAPI SetPower(int notch)
{
	g_powerNotch = notch;
}

ATS_API void WINAPI SetBrake(int notch)
{
	g_brakeNotch = notch;
}

ATS_API void WINAPI SetReverser(int pos)
{
	g_reverser = pos;
}

ATS_API void WINAPI KeyDown(int atsKeyCode)
{
	switch(atsKeyCode)
	{
	case ATS_KEY_B1: // Home: ATSリセット
		g_hk.reset();
		break;
	case ATS_KEY_B2: // End: 確認ボタン
		g_hk.confirm();
		break;
	case ATS_KEY_C1: // PageUp: 入換スイッチ
		g_hk.replace();
		break;
	}
}

ATS_API void WINAPI KeyUp(int atsKeyCode)
{
}

ATS_API void WINAPI HornBlow(int hornType)
{
}

ATS_API void WINAPI DoorOpen()
{
	g_pilotlamp = false;
	g_hk.doorState(g_pilotlamp);
}

ATS_API void WINAPI DoorClose()
{
	g_pilotlamp = true;
	g_hk.doorState(g_pilotlamp);
}

ATS_API void WINAPI SetSignal(int signal)
{
	g_hk.receive(signal);
}

ATS_API void WINAPI SetBeaconData(ATS_BEACONDATA beaconData)
{
	switch(beaconData.Type)
	{
	case ATS_BEACON_LIMIT: // 最高速度設定
		g_hk.pickLimit(beaconData.Optional);
		break;
	case ATS_BEACON_ACCEPT: // 出発承認合図
		// 2012/06/08 試験的に廃止
		//g_hk.passedAcceptTimer(beaconData.Optional);
		break;
	case ATS_BEACON_A: // 旧来A点/新型B1点
		g_hk.passedA(beaconData.Signal);
		break;
	case ATS_BEACON_S: // 旧来S点/新型B2点/入換15点
		g_hk.passedS(beaconData.Signal, beaconData.Optional);
		break;
	case ATS_BEACON_HP: // 新型HP点
		g_hk.passedHP(beaconData.Signal, beaconData.Optional);
		break;
	case ATS_BEACON_LP: // 新型B3点(LP点)
		g_hk.passedLP(beaconData.Signal);
		break;
	case ATS_BEACON_A2: // 新型A点
		g_hk.passedA2(beaconData.Optional);
		break;
	}
}
