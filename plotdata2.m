figure(2)
test2 = csvread("outputsignal.csv");
test3 = csvread("learningweights.csv");
stem(test3);
plot(test2(:,1), test2(:,2));
test2(:,1)

