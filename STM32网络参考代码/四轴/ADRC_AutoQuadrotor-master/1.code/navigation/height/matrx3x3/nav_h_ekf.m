function [ h v a ] = nav_h_ekf(Q, H, R ,F, I ,Z,up )
%���� ytf
%���� 2017��4��27��
%״̬ת��Э�������
%H Ԥ�����
%R �۲���������
%F ״̬ת�ƾ���
%I
%Z ��������

persistent X ;% ״̬����
          if(isempty(X))
           X=single([0 0 0 ]');
          end
 persistent X_ ;% ״̬����
          if(isempty(X_))
           X_=single([0 0 0 ]');
          end
          
 persistent P ;% ״̬Э�������
            if(isempty(P))
            P=single([100 0 0 ;
                      0 100 0 ;
                      0 0 100 ;]);
            end
       X_=F*X;
       P=F*P*F'+Q;
       
       if up==1
       K=P*H'*pinv(H*P*H' +R); %����
       X=X_+K*(Z-H*X_);
       P=(single(I)-K*H)*P;
       end
       
       if up==0
       H_H=H(2,:)
       K=P*H_H'*pinv(H_H*P*H_H' +R(2,2)); %����
       X=X_+K*(Z(2)-H_H*X_);
       P=(single(I)-K*H_H)*P;
       end
       
       h=X(1);
       v=X(2);
       a=X(3);
       
       
       