% Lire le fichier texte
filename = 'fichier.txt';  % Nom du fichier à traiter
data = readmatrix(filename);  % Lire les données du fichier

% Inverser les signes de la 2ème et 3ème colonne
data(:,2) = -data(:,2);  % Inverser le signe de la 2ème colonne
data(:,3) = -data(:,3);  % Inverser le signe de la 3ème colonne

% Sauvegarder les données modifiées dans un nouveau fichier
output_filename = 'resultat.txt';  % Nom du fichier de sortie
writematrix(data, output_filename, 'Delimiter', ',');  % Sauvegarder le résultat
