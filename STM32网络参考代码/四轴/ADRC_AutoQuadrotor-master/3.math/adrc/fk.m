


clc,clear
Fs = 100; %����Ƶ��
T = 1/Fs;
h = 0.008; % h,rΪ����
r=1200;
x = -2*pi:0.1:2*pi; % ����һ��������sin����
u = sin(2*x);%%.*wgn(1,length(x),10);
u1=0; % ����ֵ
u2=0;
for i=1:length(u) % �������
x1 = u1;
x2 = u2;
delta = r*h;
delta0 = delta*h;
y = x1-u+h*x2;
abs_y = abs(y);
a0 = sqrt(delta*delta+8*r*abs_y);
if (abs_y <= delta0)
a = x2+y/h; 
else 
if (y >= 0) 
a = x2 + 0.5*(a0-delta); 
else 
a = x2 - 0.5*(a0-delta); 
end
end
if (abs(a) <= delta)
fst = -r*a/delta; 
else
if (a >= 0)
fst = -r; 
else 
fst = r;
end
end
u1 = x1+T*x2; % ��������ź�
u2 = x2+T*fst; % ����΢���ź�

u1_result(i)=u1; % ������ֵ
u2_result(i)=u2; 
end

figure
plot(x,u,x,u1_result,x,u2_result)
legend('ԭʼ�ź�','�����ź�','΢���ź�')
%


