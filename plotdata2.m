%figure(2)
tic
targetsignal = csvread("targetsignal.csv");
inputsignal =csvread("inputsignalcheck.csv");
%test2 = test1(:,2);
%learningweights = csvread("learningweights.csv");
%test4 = csvread("outputsignal.csv");
%test5 = csvread("inputsignal.csv");
learningmatrix = csvread("learningmatrix.csv");
pseudoinverse = pinv(learningmatrix);
learningweights = pseudoinverse * targetsignal(:,2);





plot(learningweights);
%hold on
%plot(test1)

%figure(1)
%plot((test5))
%learning_Volterra
%plot(normc(ImpulseResponse))
toc



%plot(test3)
%stem(test3);

%plot(test2(:,1),test2(:,2),test4(:,1),test4(:,2))


