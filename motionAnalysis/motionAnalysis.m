%%
clc; clear;
filename = 'smallAirbagH1000VC'; % select raw data and video file
noice = readtable('data/noiseRefernce.txt');
noice = table2array(noice);
pressureOffset = mean(noice(:,2));
forceOffset = [mean(noice(:,3)), mean(noice(:,4)), mean(noice(:,5))];

rawData = readtable("data/"+filename+".txt");
rawData = table2array(rawData);

pressure = rawData(:,2) - pressureOffset;
forceSum = (rawData(:,3)-forceOffset(1))+(rawData(:,4)-forceOffset(2))+(rawData(:,5)-forceOffset(3));

figure(1);
subplot(2,1,1);
plot(pressure); grid on;
xlabel('time [ms]');
ylabel('pressure [mbar]');
subplot(2,1,2);
plot(forceSum); grid on;
xlabel('time [ms]');
ylabel('force [N]');

%% Initiallize video reader and player

videoReader = VideoReader(char("movies/"+filename+".mp4"));
videoPlayer = vision.VideoPlayer();

% Read frame and set tracking region
objectFrame = readFrame(videoReader);
objectRegion = [635, 40, 40, 40];
refRegion = [990, 278, 40,40];
objectImage = insertShape(objectFrame,'Rectangle',objectRegion,'Color','red');
objectImage = insertShape(objectImage,'Rectangle',refRegion,'Color','green');
figure(2);
imshow(objectImage);
title('Red and yellow box shows object reference markers');

% Detect points in the region
points = detectMinEigenFeatures(rgb2gray(objectFrame),'ROI',objectRegion);
points1 = detectMinEigenFeatures(rgb2gray(objectFrame),'ROI',refRegion);

% Display the detected points
pointImage = insertMarker(objectFrame,points.Location,'+','Color','red', 'size', 10);
pointImage = insertMarker(pointImage,points1.Location,'+','Color','green','size', 10);
figure(3);
imshow(pointImage);
title('Detected interest points');

% Create tracker object
tracker = vision.PointTracker('MaxBidirectionalError',1);
initialize(tracker,points.Location,objectFrame);
%setPoints(tracker,[1, 1]);

%% Read, track and display points on each video frame

motionData = [];
i = 1;
timeStamp = cell(1,1);
yPos = cell(1,1);

while hasFrame(videoReader)
      frame = readFrame(videoReader);
      [points,validity] = tracker(frame);
      motionData(:,i) = points(3,:);
      out = insertMarker(frame,points(validity, :),'+', 'Color','red');
      out = insertMarker(out,points1.Location,'+','Color','green');
      timeStamp{1,1} = ['Timestamp: ', num2str(i), ' ms'];
      out = insertText(out, [1000, 650], timeStamp, 'FontSize', 24);
      yPos{1,1} = ['Y position: ', num2str(round(points(3,2))), ' pixels'];
      out = insertText(out, [1000, 700], yPos, 'FontSize', 24, 'BoxColor', 'red');
      videoPlayer(out);
      i = i+1;
end

release(videoPlayer);

motionData = motionData';

%% % reverce coordinance axis 
yref = points1.Location(1,2); % extract y cord. form reference marker
yrez = 800; %video rez.
motionData(:,2) = yrez.*ones(length(motionData),1)-motionData(:,2);
yref = (yrez-yref);
%% pixels/mm scaling
y1 = 531; %[pixel]
y2 = 297; %[pixel]
realDistance_y = 252; %[mm]
scale = (y1-y2)/realDistance_y;
motionData = motionData.*scale;
yref = yref*scale;
%% set new coordinance system
realRefPlateDistance = 390; %[mm]
imparctoMarkerDistance = 91; %[mm]
platePosition = yref - realRefPlateDistance;
motionData(:,2) = motionData(:,2)-platePosition-imparctoMarkerDistance;
%%
dt = 1e-3; %[s]
displ = motionData(:,2);
t = [dt : dt : dt*length(displ)];
vel = zeros(length(displ)-1,1);
for i = 1 : length(displ)-1
   vel(i) = ( displ(i+1) - displ(i) ) / dt;
end
vel = smooth(vel);
acc = zeros(length(vel)-1,1);
for j = 1 : length(vel)-1
   acc(j) = ( vel(j+1) - vel(j) ) / dt;
end
acc = smooth(acc);
%% Ploting results
figure(4);
subplot(1,3,1);
plot(t*1e3,displ); grid on;
xlim([0 2000]);
xlabel('time [ms]');
ylabel('displacement [mm]');
subplot(1,3,2);
plot(t(2:length(t))*1e3,vel*1e-3); grid on;
xlim([0 2000]);
xlabel('time [ms]');
ylabel('velocity [m/s]');
subplot(1,3,3);
plot(t(3:length(t))*1e3,acc*1e-3); grid on;
xlim([0 2000]);
xlabel('time [ms]');
ylabel('acceleration [m/s^2]');

%% save data to csv
M = [t(1:2000)', displ(1:2000), [0; vel(1:1999)*1e-3], [0; 0; acc(1:1998)*1e-3], [pressure; pressure(end)], [forceSum; forceSum(end)]];
csvwrite("output/"+filename+".csv", M);