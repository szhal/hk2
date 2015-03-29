// 阪急電鉄 新型ATS
// Copyright (C) 2009 Unicorn

#ifndef ATS_HK_HPP_INCLUDED
#define ATS_HK_HPP_INCLUDED

#include <math.h>

#define SIGNAL_R 0 // 0
#define SIGNAL_YY 1 // 30
#define SIGNAL_Y 2 // 50
#define SIGNAL_YG 3 // 70
#define SIGNAL_G 4 // 115
#define SIGNAL_S 5 // S70
#define SIGNAL_N 6 // N

#define SPEED_R 0 // R
#define SPEED_YY 30+1 // YY
#define SPEED_Y 50+1 // Y
#define SPEED_YG 70+1 // YG
#define SPEED_S 70+1 // S1
#define SPEED_N 0 // N

#define LIMIT_F 0 // FREERUN / 15Hz
#define LIMIT_80 1 // 80km/h / 50.5Hz
#define LIMIT_70 2 // 70km/h / 20Hz
#define LIMIT_50 3 // 50km/h / 40Hz
#define LIMIT_30 4 // 30km/h / 80Hz
#define LIMIT_20 5 // 20km/h / 0Hz NOSIGNAL
#define LIMIT_20R 6 // 20km/h + OVEREB / 30Hz

#define ATSEB_NONE 0 // ATSフリー
#define ATSEB_LIMIT 1 // ATS減速後緩解
#define ATSEB_STOP 2 // ATS絶対停止

#define ROUTE_HANKYU 0 // 阪急線
#define ROUTE_HANSHIN 1 // 阪神線
#define ROUTE_SANYOU 2 // 山陽線

// #define MAX_SPEED 115+1 // 最高速度

/*
#define IND_C 0 // 透過
#define IND_0 1 // 0
#define IND_20 2 // 20
#define IND_30 3 // 30
#define IND_50 4 // 50
#define IND_70 5 // 70
#define IND_80 6 // 80
#define IND_F 7 // F
#define IND_R50 8 // 赤20<点滅>
#define IND_R70 9 // 赤30<点滅>
#define IND_R50 10 // 赤50<点滅>
#define IND_R70 11 // 赤70<点滅>
#define IND_R80 12 // 赤80<点滅>
#define IND_RF 13 // 赤F<点滅>
#define IND_P 14 // P
*/

#define IND_C 0 // 透過
#define IND_0 1 // 0
#define IND_20 2 // 20
#define IND_30 3 // 30
#define IND_50 4 // 50
#define IND_70 5 // 70
#define IND_80 6 // 80
#define IND_F 7 // F
#define IND_R50 8 // 赤50<点滅>
#define IND_R70 9 // 赤70<点滅>
#define IND_R80 10 // 赤80<点滅>
#define IND_RF 11 // 赤F<点滅>
#define IND_P 12 // P

class CHk
{
private:
	int m_door; // ドア状態
	int m_signal; // 信号現示
	int m_limitSpeed; // 最高速度

	int m_stepA; // 閉そく内A点の状態
	// int m_stepT; // 閉そく内T点の状態
	int m_stepS; // 閉そく内S点の状態
	int m_flat15; // 入換モード15km/h照査

	int m_hPat; // 高速パターン状態
	int m_lPat; // 低速パターン状態
	int m_limit; // 新A点制限状態

	float m_distHp; // 高速パターン降下までの距離[m]
	float m_distLp; // 低速パターン降下までの距離[m]

	int m_result_sig; // 信号照査の結果
	int m_result_lim; // 新A点照査の結果

	int m_leaveAccept; // 出発承認合図
	int m_leaveAcceptTime; // 出発承認合図までの時間
	int m_tmrLeaveAccept; // 出発承認合図タイマー

	int m_beginPattern; // パターン発生
	int m_confirmBuzz; // 確認モードブザー
	int m_replaceSw; // 入換スイッチ

	// 出発承認合図タイマー
	int leaveAcceptTimer()
	{
		int is_keep = 1;

		if(*Time > m_tmrLeaveAccept)
		{
			is_keep = 0; // リセットする
		}

		return is_keep;
	}

	// 表示灯をリセットする
	void resetIndicator()
	{
		Indicator = 0; // 表示器シフト
		Ats_0 = 0; // 0
		Ats_20 = 0; // 20
		Ats_30 = 0; // 30
		Ats_50 = 0; // 50
		Ats_70 = 0; // 70
		Ats_80 = 0; // 80
		Ats_F = 0; // F
		Ats_R50 = 0; //赤50<点滅>
		Ats_R70 = 0; // 赤70<点滅>
		Ats_R80 = 0; // 赤80<点滅>
		Ats_RF = 0; // 赤F<点滅>
		Ats_P = 0; // P
		Ats_N = 0; // N
		Ats_HP = 0; // HP
	}

public:
	int EmgBrake; // 非常ノッチ
	int SvcBrake; // 常用最大ノッチ

	int *BrakeNotch; // ブレーキノッチ
	int *Reverser; // レバーサ
	int *Time; // 現在時刻[s]
	float *TrainSpeed; // 速度計の速度[km/h]
	int *DeltaT; // フレーム時間[flame/ms]

	int AtsBrake; // ATSブレーキ
	int Route; // 事業者

	int Indicator; // 表示器シフト(0～12)
	// 透過-0-20-30-50-70-80-F-赤50-赤70-赤80-赤F-P(赤では点滅)

	int Ats_P; // P
	int Ats_0; // 0
	int Ats_20; // 20
	int Ats_30; // 30
	int Ats_50; // 50
	int Ats_R50; //赤50<点滅>
	int Ats_70; // 70
	int Ats_R70; // 赤70<点滅>
	int Ats_80; // 80
	int Ats_R80; // 赤80<点滅>
	int Ats_F; // F
	int Ats_RF; // 赤F<点滅>
	int Ats_N; // N
	int Ats_HP; // HP

	int Confirm; // 確認
	int Replace; // 入換

	int BeginPattern; // パターン発生
	int ConfirmBuzz; // 確認モードブザー
	int SpeedOverBuzz; // 速度超過ブザー
	int ReplaceSw; // 入換スイッチ

	// Initalizeで実行する
	void initialize()
	{
		AtsBrake = 0;
		Route = ROUTE_HANKYU;

		m_door = 0;
		m_signal = 0;
		m_limitSpeed = 115;

		m_stepA = 0;
		m_stepS = 0;
		m_flat15 = 0;
		m_hPat = 0;
		m_lPat = 0;
		m_limit = 0;
		m_result_sig = ATSEB_NONE;
		m_result_lim = ATSEB_NONE;
		m_leaveAcceptTime = 10000;
		m_beginPattern = ATS_SOUND_STOP;
		m_confirmBuzz = ATS_SOUND_STOP;
		m_replaceSw = ATS_SOUND_STOP;
	}

	// Elapseで実行する
	void execute()
	{
		float speed = fabsf(*TrainSpeed); // 速度の絶対値[km/h]
		//float def = speed / 3600 * *DeltaT; // 1フレームで動いた距離(絶対値)[m]
		float def = *TrainSpeed / 3600 * *DeltaT; // 1フレームで動いた距離(絶対値)[m]
		// 2012/08/14 距離の計算方法変更 速度の絶対値を使わない方式

		int blink = (*Time % 1000) / 500; // 表示灯点滅(0.5s)

		// 信号による照査
		switch(m_signal)
		{
		case SIGNAL_G: // フリーラン
			if(!m_stepA) // A点でない
			{
				if(speed > m_limitSpeed+1){m_result_sig = ATSEB_LIMIT;}
				else{m_result_sig = ATSEB_NONE;}
				break;
			}
		case SIGNAL_YG: // 70
			if(speed > SPEED_YG){m_result_sig = ATSEB_LIMIT;}
			else{m_result_sig = ATSEB_NONE;}
			break;
		case SIGNAL_Y: // 50
			if(!m_stepA && !m_stepS) // A点･S点でない
			// szhal : 2014/10/11
			// 変更

			//if(!m_stepA || !m_stepS) // A点･S点でない
			// unic : 2014/09/08 22:52
			// ↑ここは (!m_stepA && !m_stepS) かもしれない
			{
				if(speed > SPEED_Y){m_result_sig = ATSEB_LIMIT;}
				else{m_result_sig = ATSEB_NONE;}
				break;
			}
		case SIGNAL_YY: // 30
			if(!m_stepS) // S点でない
			{
				if(speed > SPEED_YY){m_result_sig = ATSEB_LIMIT;}
			}
			else // S点20
			{
				if(speed > 20+1){m_result_sig = ATSEB_STOP;}
			}
			break;
		case SIGNAL_S: // 70
			if(speed > SPEED_S){m_result_sig = ATSEB_LIMIT;}
			else{m_result_sig = ATSEB_NONE;}
			break;
		case SIGNAL_N: // N
		case SIGNAL_R: // 0
		default:
			if(speed > 0){m_result_sig = ATSEB_STOP;}
			break;
		}

		// 高速パターンによる照査
		if(m_hPat)
		{
			m_distHp -= def; // 残り距離を減算する
			float pattern = speed * speed / 19.5F; // パターン速度
			if(pattern >= m_distHp){m_result_sig = ATSEB_LIMIT;} // ブレーキ動作
			else{m_result_sig = ATSEB_NONE;} // 減速後緩解

			// 勾配ごとにパターン定義を変える
			// float pattern = speed * speed / (Decelation * CONST_HP0); // パターン速度

			// HPは設定距離598mの8割走行(478.4m/-119.6m)かつ停止検知でリセット
			if(m_distHp < 119.6F && m_distHp >= 10.0F && speed == 0)
			{
				m_hPat = 0;
			}
		}

		// 低速パターンによる照査
		if(m_lPat)
		{
			m_distLp -= def; // 残り距離を減算する
			float pattern = speed * speed / 19.5F; // パターン速度
			if(pattern >= m_distLp){m_result_sig = ATSEB_STOP;} // ブレーキ動作
		}

		// 新A点による照査
		switch(m_limit)
		{
		case LIMIT_20R:
			if(speed > 20+1){m_result_lim = ATSEB_STOP;}
			break;
		case LIMIT_20:
			if(speed > 20+1){m_result_lim = ATSEB_LIMIT;}
			else{m_result_lim = ATSEB_NONE;}
			break;
		case LIMIT_30:
			if(speed > 30+1){m_result_lim = ATSEB_LIMIT;}
			else{m_result_lim = ATSEB_NONE;}
			break;
		case LIMIT_50:
			if(speed > 50+1){m_result_lim = ATSEB_LIMIT;}
			else{m_result_lim = ATSEB_NONE;}
			break;
		case LIMIT_70:
			if(speed > 70+1){m_result_lim = ATSEB_LIMIT;}
			else{m_result_lim = ATSEB_NONE;}
			break;
		case LIMIT_80:
			if(speed > 80+1){m_result_lim = ATSEB_LIMIT;}
			else{m_result_lim = ATSEB_NONE;}
			break;
		case LIMIT_F:
		default:
			break;
		}

		// 表示灯をリセット
		resetIndicator();

		// 信号による表示灯
		switch(m_signal)
		{
		case SIGNAL_G:
			if(m_hPat) // 高速パターン
			{
				if(!m_door || m_distHp < 10.0F) // オーバーでNに移行
				{
					Indicator = IND_C;
					Ats_N = m_leaveAccept; // N
				}
				else // 赤F
				{
					Indicator = blink ? IND_RF : IND_C;
					Ats_RF = 1;
					Ats_HP = blink ? 1 : 0; // HP
				}
				break;
			}
			else if(!m_stepA) // フリーラン(A点でない)
			{
				Indicator = IND_F;
				Ats_F = 1;
				break;
			}
		case SIGNAL_YG: // 70
			Indicator = IND_70;
			Ats_70 = 1;
			break;
		case SIGNAL_Y: // 50
			if(!m_stepA || !m_stepS) // A点･S点でない
			{
				Indicator = IND_50;
				Ats_50 = 1;
				break;
			}
		case SIGNAL_YY: // 30
			if(!m_stepS) // S点でない
			{
				Indicator = IND_30;
				Ats_30 = 1;
			}
			else // 20
			{
				if(m_lPat) // 低速パターン
				{
					Indicator = IND_P; // P
					Ats_P = 1;
				}
				else
				{
					Indicator = IND_20;
					Ats_20 = 1;
				}
			}
			break;
		case SIGNAL_S: // 70
			Indicator = IND_70;
			Ats_70 = 1;
			break;
		case SIGNAL_N: // N
			Indicator = IND_C;
			Ats_N = 1;
			break;
		case SIGNAL_R: // 0
		default:
			Indicator = IND_0;
			Ats_0 = 1;
			break;
		}

		// 新A照査による表示灯
		switch(m_limit)
		{
		case LIMIT_20R:
		case LIMIT_20:
			if(m_signal > SIGNAL_R)
			{
				resetIndicator();
				Indicator = IND_20; // 20
				Ats_20 = 1;
			}
			break;
		case LIMIT_30:
			if(m_signal > SIGNAL_YY || (m_signal == SIGNAL_Y && !m_stepA) || !m_stepS)
			{
				resetIndicator();
				Indicator = IND_30; // 赤30
				Ats_30 = 1;
			}
			break;
		case LIMIT_50:
			if(m_signal > SIGNAL_Y || (m_signal == SIGNAL_Y && !m_stepA) || !m_stepS)
			{
				resetIndicator();
				Indicator = blink ? IND_R50 : IND_C; // 赤50
				Ats_R50 = blink ? 1 : 0;
			}
			break;
		case LIMIT_70:
			if(m_signal > SIGNAL_YG || (m_signal == SIGNAL_G && !m_stepA))
			{
				resetIndicator();
				Indicator = blink ? IND_R70 : IND_C; // 赤70
				Ats_R70 = blink ? 1 : 0;
			}
			break;
		case LIMIT_80:
			if(m_signal == SIGNAL_G && !m_stepA)
			{
				resetIndicator();
				Indicator = blink ? IND_R80 : IND_C; // 赤80
				Ats_R80 = blink ? 1 : 0;
			}
			break;
		case LIMIT_F:
		default:
			break;
		}

		// 入換モード
		if(Replace) // 30
		{
			m_result_sig = ATSEB_NONE;
			resetIndicator();

			if(m_flat15)
			{
				if(speed > 15+1){m_result_sig = ATSEB_STOP;}

				Indicator = IND_20;
				Ats_20 = 1;
			}
			else
			{
				if(speed > 30+1){m_result_sig = ATSEB_STOP;}

				Indicator = IND_30;
				Ats_30 = 1;
			}
		}

		// 確認モード
		if(Confirm) // 20
		{
			m_result_sig = ATSEB_NONE;
			resetIndicator();

			if(speed > 20+1)
			{
				m_result_sig = ATSEB_STOP;
			}

			Indicator = IND_20;
			Ats_20 = 1;
		}

		// ATSブレーキに結果を更新
		AtsBrake = max(m_result_sig, m_result_lim);

		// 出発承認合図タイマー
		if(!m_door && m_leaveAccept) // 高速パターンからN
		{
			m_leaveAccept = leaveAcceptTimer();
			if(m_leaveAccept == 0)
			{
				m_hPat = 0; // 高速パターンをリセット
			}
		}

		// 速度超過ブザー
		if(AtsBrake){SpeedOverBuzz = ATS_SOUND_PLAYLOOPING;}
		else{SpeedOverBuzz = ATS_SOUND_STOP;}

		BeginPattern = m_beginPattern; // パターン発生
		m_beginPattern = ATS_SOUND_CONTINUE;
		ConfirmBuzz = m_confirmBuzz; // 確認モードブザー
		m_confirmBuzz = ATS_SOUND_CONTINUE;
		ReplaceSw = m_replaceSw; // 入換スイッチ
		m_replaceSw = ATS_SOUND_CONTINUE;
	}

	// ドア状態が変わった時に実行する
	void doorState(bool state)
	{
		m_door = state;

		if(state == true) // 閉扉
		{
			m_leaveAccept = 0;
		}
		else // 開扉
		{
			m_tmrLeaveAccept = *Time + m_leaveAcceptTime;
		}
	}

	// SetBrakeで実行する
	void setBrake(int notch)
	{
		if(notch == EmgBrake)
		{
			Confirm = 0;
		}
	}

	// ATSリセットが扱われた時に実行する
	void reset()
	{
		if(*BrakeNotch == EmgBrake)
		{
			AtsBrake = ATSEB_NONE;
			m_result_sig = m_result_lim = ATSEB_NONE;
		}
	}

	// 確認モードが扱われた時に実行する
	void confirm(void)
	{
		if(*BrakeNotch == EmgBrake)
		{
			Confirm = 1;
			m_confirmBuzz = ATS_SOUND_PLAY;
		}
	}

	// 入換スイッチが扱われた時に実行する
	void replace()
	{
		if(*BrakeNotch == EmgBrake && (m_signal == SIGNAL_S || Replace)) // 入換信号か入換入のとき
		{
			Replace = Replace ? 0 : 1;
			m_replaceSw = ATS_SOUND_PLAY;

			//if(Replace == 0) // 入換切にした
			//{
			//	m_flat15 = 0; // フラット15
			//}
		}
	}

	// 信号現示が変わった時に実行する
	void receive(int signal)
	{
		m_signal = signal; // 信号現示
		m_stepA = 0; // 閉そく内地点の初期化
		m_stepS = 0;
		m_lPat = 0; // 低速パターンのリセット
		// add szhal 2014/10/11
		//m_stepA = m_stepS = 0; // 閉そく内地点の初期化
		//m_hPat = m_lPat = 0; // 高速パターンと低速パターンのリセット

		// 2012/06/05 試験的に廃止
		//if(signal == SIGNAL_S) // 入換信号を受けたとき確認モードを無効化
		//{
		//	Confirm = 0;
		//}
	}

	// 事業者設定を通過した時に実行する
	void pickRoute(int data=0)
	{
		Route = data % 3;
	}

	// 最高速度設定を通過した時に実行する
	void pickLimit(int data=115)
	{
		// 列選による最高速度の設定
		// 現段階では 115km/h を最高速度として固定
		// m_limitSpeed = 115;
	}

	// 出発承認合図タイマー設定を通過した時に実行する
	void passedAcceptTimer(int data)
	{
		if(*TrainSpeed > 0)
		{
			m_leaveAcceptTime = data;
		}
	}

	// A点を通過した時に実行する
	void passedA(int signal)
	{
		if(*TrainSpeed > 0)
		{
			if((m_signal == SIGNAL_G && signal == SIGNAL_Y) || (m_signal == SIGNAL_Y && signal == SIGNAL_R))
			{
				m_stepA = 1;
			}
		}
	}

	// S点を通過した時に実行する
	void passedS(int signal, int data)
	{
		if(*TrainSpeed > 0)
		{
			if(m_signal == SIGNAL_S && (signal == SIGNAL_N || data > 0)) // 入換モード
			{
				m_flat15 = 1;
			}
			else if((m_signal == SIGNAL_Y || m_signal == SIGNAL_YY) && signal == SIGNAL_R)// 通常
			{
				m_stepS = 1;
			}
		}
	}

	// HP点を通過した時に実行する
	void passedHP(int signal, int data)
	{
		if(*TrainSpeed > 0)
		{
			if(signal == SIGNAL_G) // 対照閉塞がG現示の場合
			{
				if((data == 0 || data > 2) && m_hPat == 0)
				{
					m_hPat = 1;
					m_distHp = 598; // 588[m]地点 + 10[m]余裕
					m_beginPattern = ATS_SOUND_PLAY; // パターン発生
					m_leaveAccept = 1; // 出発承認合図

					if(data)
					{
						m_leaveAcceptTime = data; // 出発承認合図タイマー
					}
				}
				else if(data == 1) // 補正用ループコイル
				{
					m_distHp = 122; // 112[m]地点 + 10[m]余裕
				}
				else if(data == 2) // 高速パターン取消
				{
					m_hPat = 0;
				}
			}
		}
	}

	// LP点を通過した時に実行する
	void passedLP(int signal)
	{
		if(*TrainSpeed > 0)
		{
			if(m_stepS == 1) // 閉そく内のS点が有効
			{
				m_lPat = 1;
				m_distLp = 31; // 31[m]地点
			}
		}
	}

	// 新A点を通過した時に時に実行する
	void passedA2(int data)
	{
		if(*TrainSpeed > 0)
		{
			if(data == 0 || m_limit < data) // リセットか照査速度落下のとき
			{
				m_limit = data;
				if(data){m_beginPattern = ATS_SOUND_PLAY;} // パターン発生
			}
		}
	}

};	//CHk

#endif	// ATS_HK_HPP_INCLUDED
