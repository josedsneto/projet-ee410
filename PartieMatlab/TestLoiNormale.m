% Données
valeurs = randn(1, 1000) * 2 + 5; % Exemple : données simulées avec une moyenne de 5 et un écart-type de 2

% Calcul de la moyenne et de l'écart-type
mu = mean(valeurs); % Moyenne
sigma = std(valeurs); % Écart-type

% Histogramme des données
figure;
histogram(valeurs, 20, 'Normalization', 'pdf'); % Histogramme normalisé en densité
hold on;

% Tracer la courbe de densité normale
x = linspace(min(valeurs), max(valeurs), 100); % Générer des points pour l'axe X
pdf_normale = (1 / (sigma * sqrt(2 * pi))) * exp(-(x - mu).^2 / (2 * sigma^2)); % Densité normale
plot(x, pdf_normale, 'r', 'LineWidth', 2); % Courbe en rouge

% Ajouter des labels
xlabel('Valeurs');
ylabel('Densité');
title('Ajustement dune loi normale');
legend('Histogramme des données', 'Courbe de densité normale');
grid on;
