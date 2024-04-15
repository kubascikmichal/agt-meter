clear all;
close all;
data = csvread("sample_data.csv");
aX = transformAccelerometer(data(:,1));
aY = transformAccelerometer(data(:,2));
aZ = transformAccelerometer(data(:,3));
gX = transformGyro(data(:,4),2);
gY = transformGyro(data(:,5),2);
gZ = transformGyro(data(:,6),2);
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

rotationX = gyroToRotation(gX,0.05);
rotationY = gyroToRotation(gY,0.05);
rotationZ = gyroToRotation(gZ,0.05);

figure('Name','Rotation data');
hold on;
Xplot = plot(rotationX);
Yplot = plot(rotationY);
Zplot = plot(rotationZ);
legend([Xplot,Yplot, Zplot], ["X","Y","Z"]);
title('Data rotacie');

filteredAccX=filterAccelerometer(aX, rotationX);
filteredAccY=filterAccelerometer(aY, rotationY);
filteredAccZ=filterAccelerometer(aZ, rotationZ);

figure('Name','Filtered accelerometer data');
hold on;
Xplot = plot(filteredAccX);
Yplot = plot(filteredAccY);
Zplot = plot(filteredAccZ);
legend([Xplot,Yplot, Zplot], ["X","Y","Z"]);
title('Data z akcelerometra');

velocityX=accelerometerToVelocity(filteredAccX,0.05);
velocityY=accelerometerToVelocity(filteredAccY,0.05);
velocityZ=accelerometerToVelocity(filteredAccZ,0.05);

figure('Name','Velocity data');
hold on;
Xplot = plot(velocityX);
Yplot = plot(velocityY);
Zplot = plot(velocityZ);
legend([Xplot,Yplot, Zplot], ["X","Y","Z"]);
title('Data rychlosti');

positionX=velocityToPosition(filteredAccY,velocityX,0.05);
positionY=velocityToPosition(filteredAccY,velocityY,0.05);
positionZ=velocityToPosition(filteredAccZ,velocityZ,0.05);

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

%make better modulo
function rotation = gyroToRotation(gyroscope, delayS)
    rotation = zeros(length(gyroscope),1);
    for i=2:1:length(gyroscope)
        rotation(i) = gyroscope(i-1)*delayS + rotation(i-1);
        if rotation(i) > 360
            rotation(i) = rotation(i) - 360;
        end
        if rotation(i) < -360
            rotation(i) = rotation(i) + 360;
        end
    end
end

function velocity = accelerometerToVelocity(accelerometer, delayS)
    velocity=zeros(length(accelerometer),1);
    for i=2:1:length(accelerometer)
        velocity(i) = accelerometer(i-1)*delayS + velocity(i-1);
    end
end

function position = velocityToPosition(accelerometer,velocity, delayS)
    position=zeros(length(accelerometer),1);
    for i=2:1:length(accelerometer)
        position(i) = (1/2)*accelerometer(i-1)*delayS*delayS + velocity(i-1)*delayS;
    end
end

function filtered = filterAccelerometer(acceleration, rotation)
    filtered=zeros(length(acceleration),1);
    for i=1:1:length(acceleration)
        filtered(i) = acceleration(i)-1/cos(rotation(i));
    end
end

function newAcc= transformAccelerometer(acc)
    newAcc=zeros(length(acc),1);
    for i=1:1:length(acc)
        newAcc(i) = acc(i)*9.81;
    end
end

function newGyro = transformGyro(gyro, digits)
    newGyro=zeros(length(gyro),1);
    for i=1:1:length(gyro)
        newGyro(i) = round(gyro(i),digits);
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