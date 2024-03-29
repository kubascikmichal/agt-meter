clear all;
close all;
data = csvread("sample_data.csv");
aX = transformAccelerometer(data(:,1));
aY = transformAccelerometer(data(:,2));
aZ = transformAccelerometer(data(:,3));
gX = transformGyro(data(:,4));
gY = transformGyro(data(:,5));
gZ = transformGyro(data(:,6));
temperature = transformTemperature(filterTemperature(data(:,7),1200), 2);

figure('Name','Accelerometer data');
hold on;
Xplot = plot(aX);
Yplot = plot(aY);
Zplot = plot(aZ);
legend([Xplot,Yplot, Zplot], ["X","Y","Z"]);
title('Data z akcelerometra');


figure('Name','Gyroscope data');
hold on;
Xplot = plot(gX);
Yplot = plot(gY);
Zplot = plot(gZ);
legend([Xplot,Yplot, Zplot], ["X","Y","Z"]);
title('Data z gyroskopu');


velocityX=accelerometerToVelocity(aX,50);
velocityY=accelerometerToVelocity(aY,50);
velocityZ=accelerometerToVelocity(aZ,50);

figure('Name','Velocity data');
hold on;
Xplot = plot(velocityX);
Yplot = plot(velocityY);
Zplot = plot(velocityZ);
legend([Xplot,Yplot, Zplot], ["X","Y","Z"]);
title('Data rychlosti');

positionX=velocityToPosition(aX, velocityX,50);
positionY=velocityToPosition(aY,velocityY,50);
positionZ=velocityToPosition(aZ,velocityZ,50);

figure('Name','Position data');
hold on;
Xplot = plot(positionX);
Yplot = plot(positionY);
Zplot = plot(positionZ);
legend([Xplot,Yplot, Zplot], ["X","Y","Z"]);
title('Data pozicie');

figure('Name','Temperature data');
hold on;
temperaturePlot = plot(temperature);
legend([temperaturePlot], ["Temperature [C]"]);
title('Data teploty');

function velocity = accelerometerToVelocity(accelerometer, delayMS)
    velocity=zeros(length(accelerometer),1);
    for i=2:1:length(accelerometer)
        velocity(i) = accelerometer(i-1)*delayMS + velocity(i-1);
    end
end

function position = velocityToPosition(accelerometer,velocity, delayMS)
    position=zeros(length(accelerometer),1);
    for i=2:1:length(accelerometer)
        position(i) = (1/2)*accelerometer(i-1)*delayMS*delayMS + velocity(i-1)*delayMS;
    end
end

function newAcc= transformAccelerometer(acc)
    newAcc=zeros(length(acc),1);
    for i=1:1:length(acc)
        newAcc(i) = acc(i)*9.81;
    end
end

function newGyro = transformGyro(gyro)
    newGyro=zeros(length(gyro),1);
    for i=1:1:length(gyro)
        newGyro(i) = mod(gyro(i),360);
    end
end

function newTemperature = transformTemperature(temp, digits)
    newTemperature=zeros(length(temp),1);
    for i=1:1:length(temp)
        newTemperature(i) = round(temp(i),digits);
    end
end

function filteredTemperature = filterTemperature(temp, windowLength)
    filteredTemperature=zeros(length(temp),1);
    for i=1:1:length(temp)-windowLength
        filteredTemperature(i) = mean(temp(i:i+windowLength));
    end
    for i=length(temp)-windowLength:1:length(temp)
        filteredTemperature(i) = mean(temp(i:length(temp)));
    end
end