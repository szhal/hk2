// その他の表示や音声
// Copyright (C) 2009 Unicorn

#ifndef ATS_SUB_HPP_INCLUDED
#define ATS_SUB_HPP_INCLUDED

class CSub
{
private:
	int m_nextBcUpdate; // BC表示更新の時刻

	// BC圧デジタル表示を更新する
	void updateBC(void)
	{
		if(*Time >= m_nextBcUpdate)
		{
			m_nextBcUpdate = *Time + 480;
			DigitalBc[0] = BcPressure < 100 ? 10 : BcPressure / 100; // Bc100の桁
			DigitalBc[1] = BcPressure < 10 ? 0 : BcPressure / 10 - (DigitalBc[0] % 10) * 10; // Bc10の桁
		}
	}

public:
	int *Time; // 現在時刻

	float BcPressure; // ブレーキシリンダ圧力[kPa]
	int DigitalBc[2]; // デジタルBC表示

	void initialize(void)
	{
		BcPressure = 0;
		DigitalBc[0] = 0;
		DigitalBc[1] = 0;
		m_nextBcUpdate = 0;
	}

	void execute(void)
	{
		updateBC(); // BC圧デジタル表示を更新する
	}
};

#endif // ATS_SUB_HPP_INCLUDED
