clc; clear all; close all;

image = imread('test6.png');
im1 = image(140:260,200:440);
[c1,x1] = imhist(im1);

image = imread('test4.png');
im2 = image(140:260,200:440);
[c2,x2] = imhist(im2);

figure(1);
subplot(2,2,1); imshow(im1);
subplot(2,2,2); imhist(im1); ylim([0 1000])
subplot(2,2,4); bar(cumsum(c1),'b'); xlim([0 256]); hold on;

tr = (640*480)*0.001;
[min_val min_loc] = min(abs(cumsum(c1)-tr));
subplot(2,2,4); plot([0 256],[tr tr],'r'); xlim([0 256])
subplot(2,2,4); plot([min_loc min_loc],[0 3e4],'r'); xlim([0 256])

im1_tr = uint8(255 .* uint8(im1 < min_loc));
subplot(2,2,3); imshow(im1_tr);


figure(2);
subplot(2,2,1); imshow(im2);
subplot(2,2,2); imhist(im2); ylim([0 1000])
subplot(2,2,4); bar(cumsum(c2),'b'); xlim([0 256]); hold on;

tr = (640*480)*0.0005;
[min_val min_loc] = min(abs(cumsum(c2)-tr));
subplot(2,2,4); plot([0 256],[tr tr],'r'); xlim([0 256])
subplot(2,2,4); plot([min_loc min_loc],[0 3e4],'r'); xlim([0 256])

im2_tr = uint8(255 .* uint8(im2 < min_loc));
subplot(2,2,3); imshow(im2_tr);