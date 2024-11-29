% Tableau de valeurs
valeurs = [3, 5, 3, 7, 2, 3, 9, 5, 5, 2, 9];

% Trouver les valeurs uniques et leur fréquence
[valeurs_uniques, ~, indices] = unique(valeurs); % Valeurs uniques et indices associés
frequence = accumarray(indices, 1); % Compte les occurrences de chaque valeur unique

% Tracer l'histogramme bâton
figure;
bar(valeurs_uniques, frequence, 'FaceColor', 'b'); % Valeurs sur X, fréquences sur Y
xlabel('Valeurs');
ylabel('Nombre dapparitions');
title('Histogramme bâton (fréquences)');
grid on;
