#define ATS_BEACON_LIMIT 0 // �ō����x�ݒ�
// #define ATS_BEACON_ACCEPT 5 // �o�����F���}
#define ATS_BEACON_A 10 // ����A�_/�V�^B1�_
#define ATS_BEACON_S 11 // ����S�_/�V�^B2�_/����15�_
#define ATS_BEACON_HP 15 // �V�^HP�_
#define ATS_BEACON_LP 16 // �V�^B3�_(LP�_)
#define ATS_BEACON_A2 17 // �V�^A�_

int g_emgBrake; // ���m�b�`
int g_svcBrake; // ��p�ő�m�b�`
int g_brakeNotch; // �u���[�L�m�b�`
int g_powerNotch; // �͍s�m�b�`
int g_reverser; // ���o�[�T
bool g_pilotlamp; // �p�C���b�g�����v
int g_time; // ���ݎ���
float g_speed; // ���x�v�̑��x[km/h]
int g_deltaT; // �t���[������[ms/frame]

ATS_HANDLES g_output; // �o��

CHk g_hk; // ��}�V�^ATS
