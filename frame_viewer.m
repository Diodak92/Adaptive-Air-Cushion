clc; clear;
filename = 'h3500_m10_vc_1'; % select raw data and video file
% play video file
filepath = fullfile('data', 'scaledCushion_1-2', '31-05-2022', 'movies', strcat(filename, '.mp4'));
implay(filepath)

%%
%videoPlayer = vision.VideoPlayer();

%while hasFrame(videoReader)
%      frame = readFrame(videoReader);
%      videoPlayer(frame);
%end