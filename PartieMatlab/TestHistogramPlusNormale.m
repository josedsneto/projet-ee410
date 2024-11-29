% Exemple de données
data = rawX_offset;  % Remplacez cette ligne par vos données

% Calculer les paramètres de la loi normale
mu = mean(data);         % Moyenne des données
sigma = std(data);       % Ecart-type des données

% Créer l'histogramme des données
figure;
histogram(data, 'Normalization', 'pdf');  % Normalisation en PDF

hold on;  % Maintenir l'histogramme pour superposer la loi normale

% Tracer la courbe de la loi normale manuellement
x = linspace(min(data), max(data), 1000);  % Créer des points pour l'axe X
y = (1 / (sigma * sqrt(2 * pi))) * exp(-0.5 * ((x - mu) / sigma).^2);  % Densité de la loi normale

plot(x, y, 'r', 'LineWidth', 2);  % Tracer la loi normale (en rouge)

% Ajouter des labels et un titre
title('Histogramme des données et loi normale correspondante');
xlabel('Valeur');
ylabel('Densité');
legend('Histogramme des données', 'Loi normale');
hold off;  % Fin du maintien de la figure
