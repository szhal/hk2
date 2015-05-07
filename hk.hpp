// 阪急電鉄 新型ATS
// Copyright (C) 2009 Unicorn

#include <math.h>

#define maximum(a,b) ((a)>(b)?(a):(b))

#define HQ_HP_DECELEATION_1 (19.5F) // 減速度2.7[km/h/s] * 7.2
#define HQ_HP_DECELEATION_2 (23.0F) // 減速度3.2[km/h/s] * 7.2
#define HQ_HP_DISTANCE_BEGIN (748.0F) // パターン開始の速度は115[km/h](588+10+150)

#define SIGNAL_R 0 // 0
#define SIGNAL_YY 1 // 30
#define SIGNAL_Y 2 // 50
#define SIGNAL_YG 3 // 70
#define SIGNAL_G 4 // 115
#define SIGNAL_S 5 // S70
#define SIGNAL_N 6 // N

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

#define IND_C 0 // 透過
#define IND_0 1 // 0
#define IND_20 2 // 20
#define IND_30 3 // 30
#define IND_50 4 // 50
#define IND_70 5 // 70
#define IND_80 6 // 80
#define IND_F 7 // F
#define IND_R20 8 // 赤20<点滅>
#define IND_R30 9 // 赤30<点滅>
#define IND_R50 10 // 赤50<点滅>
#define IND_R70 11 // 赤70<点滅>
#define IND_R80 12 // 赤80<点滅>
#define IND_RF 13 // 赤F<点滅>
#define IND_P 14 // P
#define IND_20L 15 // 20<左より>
#define IND_R20L 16 // 赤20<点滅、左より>

class CHk
{
private:
	int m_door; // ドア状態
	int m_signal; // 信号現示
	int m_limitSpeed; // 最高速度
	int m_stepA; // 閉そく内A点の状態
	int m_stepS; // 閉そく内S点の状態
	int m_flat15; // 入換モード15km/h照査

	int m_hPat; // 高速パターン状態
	int m_lPat; // 低速パターン状態
	int m_limit; // 新A点制限状態

	float m_distHp; // 高速パターン降下までの距離[m]
	float m_distLp; // 低速パターン降下までの距離[m]
	float m_hpDeceleation; // 高速パターン減速定数

	int m_result_sig; // 信号照査の結果
	int m_result_hp; // 高速パターンの結果
	int m_result_lim; // 新A点照査の結果

	int m_lpRelease; // LP解除タイマー
	int m_lpReleaseTime; // LP解除までの時間

	int m_nextBcUpdate; // BC表示更新の時刻

	int m_beginPattern; // パターン発生
	// int m_confirmBuzz; // 確認モードブザー
	int m_replaceSw; // 入換スイッチ
	int m_confirmButton; // 確認ボタン

	// LP解除タイマー
	int lpReleaseTimer()
	{
		int is_keep = 1;

		if(*Time >= m_lpReleaseTime)
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
		Ats_R20 = 0; //赤20<点滅>
		Ats_R30 = 0; //赤30<点滅>
		Ats_R50 = 0; //赤50<点滅>
		Ats_R70 = 0; // 赤70<点滅>
		Ats_R80 = 0; // 赤80<点滅>
		// Ats_RF = 0; // 赤F<点滅>
		Ats_P = 0; // P
		Ats_N = 0; // N
		Ats_HP = 0; // HP
		Ats_RN = 0; // 赤N
		Ats_Confirm = 0; // 確認<点滅>
	}

public:
	int EmgBrake; // 非常ノッチ
	int SvcBrake; // 常用最大ノッチ

	int *BrakeNotch; // ブレーキノッチ
	int *Reverser; // レバーサ
	int *Time; // 現在時刻[s]
	float *TrainSpeed; // 速度計の速度[km/h]
	int *DeltaT; // フレーム時間[flame/ms]
	float BcPressure; // ブレーキシリンダ圧力[kPa]
	int DigitalBc[2]; // デジタルBC表示

	int AtsBrake; // ATSブレーキ
	int Route; // 事業者

	int Indicator; // 表示器シフト
	// 透過-0-20-30-50-70-80-F-赤20-赤30-赤50-赤70-赤80-赤F-P-20N(赤では点滅)

	int Ats_P; // P
	int Ats_0; // 0
	int Ats_20; // 20
	int Ats_R20; // 赤20<点滅>
	int Ats_30; // 30
	int Ats_R30; // 赤30<点滅>
	int Ats_50; // 50
	int Ats_R50; //赤50<点滅>
	int Ats_70; // 70
	int Ats_R70; // 赤70<点滅>
	int Ats_80; // 80
	int Ats_R80; // 赤80<点滅>
	int Ats_F; // F
	// int Ats_RF; // 赤F<点滅>
	int Ats_N; // N
	int Ats_HP; // HP
	int Ats_RN; // 赤N
	int Ats_Confirm; // 確認<点滅>

	int Confirm; // 確認
	int Replace; // 入換

	int BeginPattern; // パターン発生
	int ConfirmBuzz; // 確認モードブザー
	int SpeedOverBuzz; // 速度超過ブザー
	int ReplaceSw; // 入換スイッチ
	int ConfirmButton; // 確認ボタン

	// Initalizeで実行する
	void initialize()
	{
		AtsBrake = 0;

		m_door = 0;
		m_signal = 0;
		m_limitSpeed = 115;

		m_stepA = 0;
		m_stepS = 0;
		m_flat15 = 0;
		m_hPat = 0;
		m_lPat = 0;
		m_limit = 0;

		m_distHp = 0;
		m_distLp = 0;
		m_hpDeceleation = HQ_HP_DECELEATION_1; // 高速パターン減速定数

		m_result_sig = ATSEB_NONE;
		m_result_hp = ATSEB_NONE;
		m_result_lim = ATSEB_NONE;

		m_lpRelease = 0;
		m_lpReleaseTime = 0;

		BcPressure = 0;
		DigitalBc[0] = 0;
		DigitalBc[1] = 0;
		m_nextBcUpdate = 0;

		m_beginPattern = ATS_SOUND_STOP;
		m_replaceSw = ATS_SOUND_STOP;
		m_confirmButton = ATS_SOUND_STOP;
	}

	// Elapseで実行する
	void execute()
	{
		float speed = fabsf(*TrainSpeed); // 速度の絶対値[km/h]
		float def = *TrainSpeed / 3600 * *DeltaT; // 1フレームで動いた距離(絶対値)[m]

		int blink500 = (*Time % 1000) / 500; // 表示灯点滅(0.5s)

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
			if(speed > 70+1){m_result_sig = ATSEB_LIMIT;}
			else{m_result_sig = ATSEB_NONE;}
			break;
		case SIGNAL_Y: // 50
			if(!m_stepA && !m_stepS) // A点･S点でない
			{
				if(speed > 50+1){m_result_sig = ATSEB_LIMIT;}
				else{m_result_sig = ATSEB_NONE;}
				break;
			}
		case SIGNAL_YY: // 30
		case SIGNAL_S: // REPLACE
			if(!m_stepS) // S点でない
			{
				if(speed > 30+1){m_result_sig = ATSEB_LIMIT;}
				else{m_result_sig = ATSEB_NONE;}
			}
			else // S点20
			{
				// 2015.5.5 SzHaL 通常のS点では緩解
				if(speed > 20+1){m_result_sig = ATSEB_LIMIT;}
				else{m_result_sig = ATSEB_NONE;}
			}
			break;
		case SIGNAL_N: // N
				// 2015.5.5 SzHaL ATS無信号区間は20km/h照査
				if(speed > 20+1){m_result_sig = ATSEB_LIMIT;}
				else{m_result_sig = ATSEB_NONE;}
			break;

		case SIGNAL_R: // 0
		default:
			if(speed > 0){m_result_sig = ATSEB_STOP;}
			break;
		}

		// 高速パターンによる照査
		if(m_hPat)
		{
			m_distHp -= def; // 残り距離を減算する
			float pattern = speed * speed / m_hpDeceleation; // パターン速度
			if(pattern >= m_distHp){m_result_hp = ATSEB_LIMIT;} // ブレーキ動作
			else{m_result_hp = ATSEB_NONE;} // 減速後緩解

			// HPは残距離119.6m未満かつ3km/h以下でリセット
			// 2015.5.3 リセット条件を速度3km/h以下に変更と、パターン終端超えた場合は非常制動に。
			if(m_distHp <= 10.0F)
			{
				m_result_hp = ATSEB_STOP;
			}
			else if(m_distHp < 119.6F && m_distHp >= 10.0F && speed <= 3.0F)
			{
				m_hPat = 0;
				m_result_hp = ATSEB_NONE; // 2015.5.5 SzHaL 高速パターンキャンセル後にブレーキ緩解せず
			}
		}

		// 低速パターンによる照査
		if(m_lPat || m_lpRelease)
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
			if(!m_stepA) // フリーラン(A点でない)
			{
				Indicator = m_hPat ? IND_RF * blink500 : IND_F; // F
				Ats_F = m_hPat ? blink500 : 1;

				/*
				if(m_hPat) // 高速パターン
				{
					Ats_F = 0;
					// Ats_RF = blink500 ? 1 : 0; //2015.5.3 SzHal 高速パターン時F点滅を追加
				}
				*/
				break;
			}
		case SIGNAL_YG: // 70
			Indicator = m_hPat ? IND_R70 * blink500 : IND_70; // 70
			Ats_70 = 1;
			Ats_R70 = m_hPat ? blink500 : 0;

			/*
			if(m_hPat) // 高速パターン
			{
				Ats_R70 = blink500 ? 1 : 0;
			}
			*/
			break;
		case SIGNAL_Y: // 50
			if(!m_stepA && !m_stepS) // A点･S点でない
			{
				Indicator = m_hPat ? IND_R50 * blink500 : IND_50; // 50
				Ats_50 = 1;
				Ats_R50 = m_hPat ? blink500 : 0;
				
				/*
				if(m_hPat) // 高速パターン
				{
					Ats_R50 = blink500 ? 1 : 0;
				}
				*/
				break;
			}
		case SIGNAL_YY: // 30
			if(!m_stepS) // S点でない
			{
				Indicator = m_hPat ? IND_R30 * blink500 : IND_30; // 30
				Ats_30 = 1;
				Ats_R30 = m_hPat ? blink500 : 0;

				/*
				if(m_hPat) // 高速パターン
				{
					Ats_R30 = blink500 ? 1 : 0;
				}
				*/
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
					if(m_hPat) // 高速パターンでは赤20(左より)点滅と赤N(右より)点灯
					{
						Indicator = IND_R20L * blink500;
						Ats_RN = 1;
						Ats_20 = 1;
						Ats_R20 = blink500;
					}
					else // それ以外では白20(左より)点灯と赤N(右より)点灯
					{
						// 2015.5.5 SzHaL 停止時N表示は廃止
						Indicator = IND_20L;
						Ats_RN = 1;
						Ats_20 = 1;
					}
				}
			}
			break;
		case SIGNAL_S: // REPLACE
			// 2015.5.5 SzHaL 資料には入換信号受信の合図がATS70との表記が…(汗)
			Indicator = IND_70;
			Ats_70 = 1;
			break;
		case SIGNAL_N: // N
		case SIGNAL_R: // 0
		default:
			Indicator = IND_C;
			Ats_N = 1;
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
				Indicator = m_hPat ? IND_R20 * blink500 : IND_20; // 20
				Ats_HP = m_hPat ? blink500 : 0; // HP
				Ats_R20 = m_hPat ? blink500 : 0;
				Ats_20 = 1;

				/*
				if(m_hPat) // 高速パターン
				{
					resetIndicator();
					Ats_HP = blink500 ? 1 : 0; // HP
					Ats_R20 = blink500 ? 1 : 0;
					Ats_20 = 1;
					Indicator = IND_R20 * blink500;
				}
				*/
			}
			break;
		case LIMIT_30:
			if(m_signal > SIGNAL_YY || (m_signal == SIGNAL_Y && !m_stepA) || !m_stepS)
			{
				resetIndicator();
				Indicator = m_hPat ? IND_R30 * blink500 : IND_30; // 30
				Ats_HP = m_hPat ? blink500 : 0; // HP
				Ats_R30 = m_hPat ? blink500 : 0;
				Ats_30 = 1;

				/*
				if(m_hPat) // 高速パターン
				{
					resetIndicator();
					Ats_HP = blink500 ? 1 : 0; // HP
					Ats_R30 = blink500 ? 1 : 0;
					Ats_30 = 1;
					Indicator = IND_R30 * blink500;
				}
				*/
			}
			break;
		case LIMIT_50:
			if(m_signal > SIGNAL_Y || (m_signal == SIGNAL_Y && !m_stepA) || !m_stepS)
			{
				resetIndicator();
				Indicator = m_hPat ? IND_R50 * blink500 : IND_50; // 50
				Ats_HP = m_hPat ? blink500 : 0; // HP
				Ats_R50 = m_hPat ? blink500 : 0;
				Ats_50 = 1;

				/*
				if(m_hPat) // 高速パターン
				{
					resetIndicator();
					Ats_HP = blink500 ? 1 : 0; // HP
					Ats_R50 = blink500 ? 1 : 0;
					Ats_50 = 1;
					Indicator = IND_R50 * blink500;
				}
				*/
			}
			break;
		case LIMIT_70:
			if(m_signal > SIGNAL_YG || (m_signal == SIGNAL_G && !m_stepA))
			{
				resetIndicator();
				Indicator = m_hPat ? IND_R70 * blink500 : IND_70; // 70
				Ats_HP = m_hPat ? blink500 : 0; // HP
				Ats_R70 = m_hPat ? blink500 : 0;
				Ats_70 = 1;

				/*
				if(m_hPat) // 高速パターン
				{
					resetIndicator();
					Ats_HP = blink500 ? 1 : 0; // HP
					Ats_R70 = blink500 ? 1 : 0;
					Ats_70 = 1;
					Indicator = IND_R70 * blink500;
				}
				*/
			}
			break;
		case LIMIT_80:
			if(m_signal == SIGNAL_G && !m_stepA)
			{
				resetIndicator();
				Indicator = m_hPat ? IND_R80 * blink500 : IND_80; // 80
				Ats_HP = m_hPat ? blink500 : 0; // HP
				Ats_R80 = m_hPat ? blink500 : 0;
				Ats_80 = 1;

				/*
				if(m_hPat) // 高速パターン
				{
					resetIndicator();
					Ats_HP = blink500 ? 1 : 0; // HP
					Ats_R80 = blink500 ? 1 : 0;
					Ats_80 = 1;
					Indicator = IND_R80 * blink500;
				}
				*/
			}
			break;
		case LIMIT_F:
		default:
			break;
		}

		// 高速パターンによる表示灯
		// 2015.5.5 SzHaL 先に処理するとN表示時にF点滅消えず
		if(m_hPat) // 高速パターン
		{
			// if(!m_door || m_distHp < 10.0F) // オーバーでNに移行
			// 2015/05/04 変更 unic ドアを見ない
			if(m_distHp < 10.0F) // オーバーでNに移行
			{
				resetIndicator();
				Indicator = IND_C;
				Ats_N = 1; // N
			}
			else // 赤F
			{
				Ats_HP = blink500 ? 1 : 0; // HP
			}
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
			//2015.4.29 SzHal 10m以上過走した際のATSブレーキの強制解除
			m_hPat = 0;
			m_result_hp = ATSEB_NONE;

			resetIndicator();

			if(speed > 20+1)
			{
				m_result_sig = ATSEB_STOP;
			}

			Indicator = IND_20;
			Ats_20 = 1;
			Ats_Confirm = blink500;
		}
		else
		{
			Ats_Confirm = 0;
		}

		// LP解除タイマーの表示
		if(m_lpRelease) // LP地上子からタイマーを使う設定があるとき
		{
			if(lpReleaseTimer())
			{
				m_lpRelease = 0;
				resetIndicator();
				Indicator = IND_P; // P
				Ats_P = 1;
			}
		}

		// ATSブレーキに結果を更新
		AtsBrake = maximum(maximum(m_result_sig,m_result_hp), m_result_lim);

		// 速度超過ブザー
		SpeedOverBuzz = AtsBrake ? ATS_SOUND_PLAYLOOPING : ATS_SOUND_STOP;
		ConfirmBuzz = Confirm ? ATS_SOUND_PLAYLOOPING : ATS_SOUND_STOP;

		BeginPattern = m_beginPattern; // パターン発生
		m_beginPattern = ATS_SOUND_CONTINUE;
		ReplaceSw = m_replaceSw; // 入換スイッチ
		m_replaceSw = ATS_SOUND_CONTINUE;
		ConfirmButton = m_confirmButton; // 確認ボタン
		m_confirmButton = ATS_SOUND_CONTINUE;

		// BC圧デジタル表示
		if(*Time >= m_nextBcUpdate)
		{
			m_nextBcUpdate = *Time + 480;
			DigitalBc[0] = BcPressure < 100 ? 10 : BcPressure / 100; // Bc100の桁
			DigitalBc[1] = BcPressure < 10 ? 0 : BcPressure / 10 - (DigitalBc[0] % 10) * 10; // Bc10の桁
		}
	}

	// ドア状態が変わった時に実行する
	void doorState(bool state)
	{
		m_door = state;

/*
		if(state == true) // 閉扉
		{
			m_lpRelease = 0;
		}
*/
	}

	// ATSリセットが扱われた時に実行する
	void reset(void)
	{
		if(*BrakeNotch == EmgBrake)
		{
			AtsBrake = ATSEB_NONE;

			// 2015.5.3 SzHaL N表示後、力行が必要な時のS標20照査解除
			m_result_sig = ATSEB_NONE;
			m_result_hp = ATSEB_NONE;
			m_result_lim = ATSEB_NONE;
			m_stepA = 0;
			m_stepS = 0;
		}
	}

	// 確認モードが扱われた時に実行する
	void confirm(void)
	{
		if(*BrakeNotch == EmgBrake)
		{
			m_confirmButton = ATS_SOUND_PLAY;

			if(Confirm == 0)
			{
				Confirm = 1;
				// m_confirmBuzz = ATS_SOUND_PLAYLOOPING;

				m_hPat = m_lPat = 0;
				AtsBrake = ATSEB_NONE;
				m_result_sig = ATSEB_NONE;
				m_result_hp = ATSEB_NONE;
				m_result_lim = ATSEB_NONE;
			}
			else
			{
				Confirm = 0;
				// m_confirmBuzz = ATS_SOUND_STOP;
			}
		}
	}

	// 入換スイッチが扱われた時に実行する
	void replace(void)
	{
		if(*BrakeNotch == EmgBrake && (m_signal == SIGNAL_S || Replace)) // 入換信号か入換入のとき
		{
			Replace = Replace ? 0 : 1;
			m_replaceSw = ATS_SOUND_PLAY;
		}
	}

	// 信号現示が変わった時に実行する
	void receive(int signal)
	{
		// 信号10番は、A点有効時のG→Gアップ取得用
		m_signal = (signal == 10) ? SIGNAL_G : signal; // 信号現示

		m_stepA = 0; // 閉そく内地点の初期化
		m_stepS = 0;
		m_lPat = 0; // 低速パターンのリセット
	}

	// 最高速度設定を通過した時に実行する
	void pickLimit(int data)
	{
		if(data == 1) // 宝塚線
		{
			m_limitSpeed = 100;
		}
		else // それ以外
		{
			m_limitSpeed = 115;
		}
	}

	// 出発承認合図タイマー設定を通過した時に実行する
	/*
	void passedAcceptTimer(int data)
	{
		if(*TrainSpeed > 0)
		{
			m_lpReleaseTime = *Time + data;
		}
	}
	*/

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
			if(signal == SIGNAL_R && data > 0) // 入換モード
			{
				m_flat15 = 1;
			}

			if((m_signal == SIGNAL_YY || (m_signal == SIGNAL_Y && m_stepA)) && signal == SIGNAL_R) // 通常
			{
				m_stepS = 1;
				if(Replace){Replace = 0;} // 入換モード無効化
			}
		}
	}

	// HP点を通過した時に実行する
	void passedHP(int signal, int data)
	{
		if(*TrainSpeed > 0)
		{
			switch(data)
			{
			case 0:
				if(m_hPat == 0)
				{
					m_hPat = 1;
					m_distHp = HQ_HP_DISTANCE_BEGIN; // パターン開始の速度を決める定数
					m_hpDeceleation = HQ_HP_DECELEATION_1;
					m_beginPattern = ATS_SOUND_PLAY; // パターン発生
				}
				break;
			case 1: // 補正用ループコイル
				m_distHp = 122.0F; // 112[m]地点 + 10[m]余裕
				break;
			case 2: // 補正用ループコイル(200m)
				m_distHp = 270.0F;
				m_hpDeceleation = HQ_HP_DECELEATION_2;
				break;
			case 3: // 高速パターン取消
				m_hPat = 0;
				m_result_hp = ATSEB_NONE;
				break;
			}
		}
	}

	// LP点を通過した時に実行する
	void passedLP(int signal, int data)
	{
		if(*TrainSpeed > 0)
		{
			if(m_stepS == 1) // 閉そく内のS点が有効
			{
				m_lPat = 1;
				m_distLp = 31.0F; // 31[m]地点

				if(data > 0)
				{
					m_lpRelease = 1; // LP解除タイマー
					m_lpReleaseTime = data + *Time; // LP解除までの時間
				}
			}
		}
	}

	// 新A点を通過した時に時に実行する
	void passedA2(int data)
	{
		if(data == 0 || m_limit < data) // リセットか照査速度落下のとき
		{
			m_limit = data;
		}
	}
};	// CHk
