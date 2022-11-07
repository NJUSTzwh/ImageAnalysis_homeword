f = checkerboard(8);

PSF = fspecial('motion', 7, 45);

gb = imfilter(f, PSF, 'circular');

noise = imnoise(zeros(size(f)), 'gaussian', 0, 0.001);

g = gb + noise;

figure
subplot(1, 2, 1)
imshow(pixeldup(g, 8), []);
title('(1)含有噪声的退化图像');

fr1 = deconvwnr(g, PSF);
subplot(1, 2, 2)
imshow(pixeldup(fr1, 8), [ ])
title('(2)逆滤波结果')

Sn = abs(fft2(noise)).^2;
nA = sum(Sn(:)) / prod(size(noise));
Sf = abs(fft2(f)).^2;
fA = sum(Sf(:)) / prod(size(f));
R = nA / fA;
fr2 = deconvwnr(g, PSF, R);
figure
subplot(1, 2, 1);
imshow(pixeldup(fr2, 8), [ ])
title('(3)维纳滤波结果(信噪比为常数)');

NCORR = fftshift(real(ifft2(Sn)));
ICORR = fftshift(real(ifft2(Sf)));
fr3 = deconvwnr(g, PSF, NCORR, ICORR);
subplot(1, 2, 2)
imshow(pixeldup(fr3, 8), [])
title('(4)维纳滤波结果(信噪比为非常数)');
