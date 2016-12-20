clc; clear all; 
% close all;

image = imread('test4.png');
image = image(140:260,200:440);
figure(1); subplot(1,2,1); imshow(image)

image_tr = uint8(255 .* uint8(image < 70));
figure(1); subplot(1,2,2); imshow(image_tr);return

g = imfill(image_tr,'holes');
% figure(3);imshow(g)

[rows cols] = find(g);

rbar = round(mean(rows));
cbar = round(mean(cols));

image(:,cbar) = ~g(:,cbar);
image(rbar,:) = ~g(rbar,:);
figure(4); imshow(image)
