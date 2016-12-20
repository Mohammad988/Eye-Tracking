clc; clear all; 
% close all;

image = imread('test6.png');
image = image(140:260,200:440);
figure(1);subplot(1,2,1); imshow(image)

framesize = size(image,1)*size(image,2);
[PDF, x] = imhist(image);
CDF = cumsum(PDF)/framesize;

image_tr = uint8(255 .* ((uint8((CDF(image) < 0.005).*double(image)))>1));
figure(1);subplot(1,2,2); imshow(image_tr)

g = imfill(image_tr,'holes');
% figure(3);imshow(g)

[rows cols] = find(g);

rbar = round(mean(rows));
cbar = round(mean(cols));

image(:,cbar) = ~g(:,cbar);
image(rbar,:) = ~g(rbar,:);
figure(4); imshow(image)

% nz_cdf = CDF(PDF > 0);
% cdf_eq = zeros(size(CDF));
% for i = 1:length(PDF)
%     if(PDF(i) > 0)
%         cdf_eq(i) = round((CDF(i)-min(nz_cdf)) ...
%                         /(framesize-min(nz_cdf))*255);
%     end
% end
% 
% for i = 1:size(image,1)
%     for j = 1:size(image,2)
%         image_eq(i,j) = uint8(cdf_eq(image(i,j)));
%     end
% end
% 
% figure(3); imshow(image_eq)
% 
% [PDF, x] = imhist(image_eq);
% CDF = cumsum(PDF);
% figure(4); clf; hold on;
% stem(x,PDF/framesize*50)
% plot(x,CDF/framesize,'r')