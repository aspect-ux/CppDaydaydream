#pragma once

// 1. ��ͨ��,�����ǳ���������������
#define PI 3.1415926

// 2. ���κ���
#define SQUARE(x) (x * x)

// 3. �������룬ֻ���壬ͨ��if else���ƿ���
#define DEBUG


// []��ʾ����capture�������ı�����[=]��ֵ��[&]�����ã�[]ʲôҲ������; thisָ��ֻ���ڳ�Ա������ʹ��,�����ھ�̬������ʹ��
#define AS_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }