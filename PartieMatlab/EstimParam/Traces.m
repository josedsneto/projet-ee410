% Tracés en plus
nb = 6;
close all;
tracerLoiNormaleHistogram(rawX_offset*x, 8, "0g offset axe X, ADXL345");
tracerLoiNormaleHistogram(rawY_offset*y, 6, "0g offset axe Y, ADXL345");
tracerLoiNormaleHistogram(rawZ_offset*z, 6, "0g offset axe Z, ADXL345");

%% Functions


function tracerLoiNormaleHistogram(data, nbHistogram, titre)
    % Calculer les paramètres de la loi normale
    mu = mean(data);         % Moyenne des données
    sigma = std(data);       % Ecart-type des données
    
    % Créer l'histogramme des données
    figure;
    histogram(data, nbHistogram, 'Normalization', 'pdf');  % Normalisation en PDF
    
    hold on;  % Maintenir l'histogramme pour superposer la loi normale
    
    % Tracer la courbe de la loi normale manuellement
    x = linspace(min(data), max(data), 1000);  % Créer des points pour l'axe X
    y = (1 / (sigma * sqrt(2 * pi))) * exp(-0.5 * ((x - mu) / sigma).^2);  % Densité de la loi normale
    
    plot(x, y, 'r', 'LineWidth', 2);  % Tracer la loi normale (en rouge)
    
    % Ajouter des labels et un titre
    title(titre);
    xlabel('Valeur en g (range : +- 2g)');
    ylabel('Densité');
    legend('Histogramme des données', 'Loi normale');
    hold off;  % Fin du maintien de la figure

end