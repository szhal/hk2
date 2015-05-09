// ���̑��̕\���≹��
// Copyright (C) 2009 Unicorn

#ifndef ATS_SUB_HPP_INCLUDED
#define ATS_SUB_HPP_INCLUDED

class CSub
{
private:
	int m_nextBcUpdate; // BC�\���X�V�̎���

	// BC���f�W�^���\�����X�V����
	void updateBC(void)
	{
		if(*Time >= m_nextBcUpdate)
		{
			m_nextBcUpdate = *Time + 480;
			DigitalBc[0] = BcPressure < 100 ? 10 : BcPressure / 100; // Bc100�̌�
			DigitalBc[1] = BcPressure < 10 ? 0 : BcPressure / 10 - (DigitalBc[0] % 10) * 10; // Bc10�̌�
		}
	}

public:
	int *Time; // ���ݎ���

	float BcPressure; // �u���[�L�V�����_����[kPa]
	int DigitalBc[2]; // �f�W�^��BC�\��

	void initialize(void)
	{
		BcPressure = 0;
		DigitalBc[0] = 0;
		DigitalBc[1] = 0;
		m_nextBcUpdate = 0;
	}

	void execute(void)
	{
		updateBC(); // BC���f�W�^���\�����X�V����
	}
};

#endif // ATS_SUB_HPP_INCLUDED
