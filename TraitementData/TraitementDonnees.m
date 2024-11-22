% ========== ANALYSE DONNES CAPTEUR DE CHUTE ==========
close all;
M = readmatrix("PleinChute2.txt_formate.txt");
module = M(:, 4);
buttonValue = M(:,5);

nbPTUsefullData = numberOfPointsUsefulData(buttonValue);
nbPT = length(module);

% figure; grid on;
% plot(1:nbPT, module, "b",1:nbPT, buttonValue.*module, "r");


%module = [1,2,1,2,-3,-4,0,5,4,3,-2,0,1,1, 1,2,1,2,-3,-4,0,5,4,3,-2,0,1,1];
%buttonValue = [0,0,0,1,1,1,1,1,1,1,1,1,0,0, 0,0,0,1,1,1,1,1,1,1,1,1,0,0];

[minVector, indexMinVector, maxVector, indexMaxVector] = findPeaks(module, buttonValue);

%nbPT = length(module);
figure; grid on;hold on;
plot(1:nbPT, module, "b",1:nbPT, buttonValue.*module, "r");
hold on;
plot([indexMinVector, indexMaxVector], [minVector, maxVector], "o",'MarkerSize', 4, 'MarkerFaceColor', 'b');
hold off;

%% Test sur l'histogram


% close all;
% for i=4:2:14
%     figure;
%     subplot(1,2,1);
%     histogram(minVector, i);
%     title("Minimums");
%     subplot(1,2,2);
%     histogram(maxVector, i);
%     title("Maximums");
% end

% Calcul de la moyenne et de l'écart-type
mu_max = mean(maxVector);mu_min = mean(minVector); % Moyenne
sigma_max = std(maxVector);sigma_min = std(minVector); % Écart-type

% Tracer la courbe de densité normale
x_max = linspace(min(maxVector), max(maxVector), 100); % Générer des points pour l'axe X
pdf_normale_max = (1 / (sigma_max * sqrt(2 * pi))) * exp(-((x_max - mu_max).^2) / (2 * sigma_max^2)); % Densité normale
x_min = linspace(min(minVector), max(minVector), 100); % Générer des points pour l'axe X
pdf_normale_min = (1 / (sigma_min * sqrt(2 * pi))) * exp(-((x_min - mu_min).^2) / (2 * sigma_min^2)); % Densité normale


figure;
subplot(1,2,1); hold on;
histogram(minVector, 6, 'Normalization','probability');
%h = histogram(minVector, 6, 'Normalization','probability');
%pdf_normale_min = max(h.Values)/max(pdf_normale_min);
pdf_normale_min = pdf_normale_min/8*0.3;
plot(x_min, pdf_normale_min, 'r', 'LineWidth', 2); % Courbe en rouge
title("Pics Minimums");

subplot(1,2,2); hold on;
histogram(maxVector, 6, 'Normalization','probability');
pdf_normale_max = pdf_normale_max/1.5*0.35;
plot(x_max, pdf_normale_max, 'r', 'LineWidth', 2); % Courbe en rouge
title("Pics Maximums");


%% Functions
true;
function nb = numberOfPointsUsefulData(vectorButton)
    nb = sum(vectorButton);
end

function nb = numberOfFalls(buttonValue)
    counter = 0; 
    for i=2:(length(buttonValue)-1)
        if (buttonValue(i-1) < buttonValue(i))
            counter = counter + 1;
        end
    end
    nb = counter;

end


function [vector] = getSegmentIndex(buttonValue)
    vector = zeros(1, 2*numberOfFalls(buttonValue));
    indexVector = 1;
    for i=2:(length(buttonValue)-1)
        if (buttonValue(i-1) < buttonValue(i))
            vector(indexVector) = i;
            indexVector = indexVector +1;
        end
        if (buttonValue(i-1) > buttonValue(i))
            vector(indexVector) = i-1;
            indexVector = indexVector +1;
        end
    end
    
end



function [minVector, indexMinVector, maxVector, indexMaxVector] = findPeaks(module, buttonValue)
    nbSection = numberOfFalls(buttonValue);
    seg = getSegmentIndex(buttonValue);
    minVector = zeros(1, nbSection); maxVector = zeros(1, nbSection);
    indexMinVector = zeros(1, nbSection); indexMaxVector = zeros(1, nbSection);

    for i=1:nbSection
        % Finding the max
        [a,b] = max(module(seg(2*i-1):seg(2*i))); % a = max; b = index (relative)
        maxVector(i) = a;
        indexMaxVector(i) = b+seg(2*i-1)-1; % absolute index

        [a,b] = min(module(seg(2*i-1):indexMaxVector(i))); % Le min qu'on cherche se trouve avt le max
        minVector(i) = a;
        indexMinVector(i) = b+seg(2*i-1)-1; % absolute index
    end
end

