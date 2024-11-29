%% ========== ESTIMATION DES PARAMETRE DE L'ADXL 345 ==========
close all; 
DataX = readmatrix("AxeX345.txt_formate.txt");
DataY = readmatrix("AxeY345.txt_formate.txt");
DataZ = readmatrix("AxeZ345.txt_formate.txt");

%% ========== Offsets ==========
% === Calcul de l'offset sur X
dataY_x = DataY(:, 1); dataZ_x = DataZ(:,1);
rawX_offset = [dataY_x; dataZ_x];
tracerLoiNormaleHistogram(rawX_offset, 12, "0g offset axe X, ADXL345");
x_offset = mean(rawX_offset)

% === Calcul de l'offset sur Y
rawY_offset = [DataX(:,2); DataZ(:,2)];
tracerLoiNormaleHistogram(rawY_offset, 12, "0g offset axe Y, ADXL345");
y_offset = mean(rawY_offset)

% === Calcul de l'offset sur Z
rawZ_offset = [DataX(:,3); DataY(:,3)];
tracerLoiNormaleHistogram(rawZ_offset, 12, "0g offset axe Z, ADXL345");
z_offset = mean(rawZ_offset)

% ====== ADXL345
% x_offset =   -0.8703
% y_offset =    0.8459
% z_offset =    0.8846

% ====== ADXL359
% x_offset =  165.5092
% y_offset =   69.0781
% z_offset = -316.8318




%% ========== Gains ==========
raw_x1 = mean(DataX(:,1)); 
raw_y1 = mean(DataX(:,2));
raw_z1 = mean(DataX(:,3));
raw_x2 = mean(DataY(:,1));
raw_y2 = mean(DataY(:,2));
raw_z2 = mean(DataY(:,3));
raw_x3 = mean(DataZ(:,1));
raw_y3 = mean(DataZ(:,2));
raw_z3 = mean(DataZ(:,3));

% Paramètres donnés pour résoudre l'équation 
% sqrt( (ax)²+(by)² + (cz²)) = 1
% sqrt (dx)² + (ey)² + (fz²)) = 1
% sqrt( (gx)² + (hy)² + (iz)²) = 1

a = raw_x1-x_offset; b = raw_y1-y_offset; c = raw_z1-z_offset; % Exemple de valeurs
d = raw_x2-x_offset; e = raw_y2-y_offset; f = raw_z2-z_offset; % Exemple de valeurs
g = raw_x3-x_offset; h = raw_y3-y_offset; i = raw_z3-z_offset; % Exemple de valeurs

% Définir les équations sous forme de fonctions anonymes
equations = @(vars) [
    sqrt((a * vars(1))^2 + (b * vars(2))^2 + (c * vars(3))^2) - 1;
    sqrt((d * vars(1))^2 + (e * vars(2))^2 + (f * vars(3))^2) - 1;
    sqrt((g * vars(1))^2 + (h * vars(2))^2 + (i * vars(3))^2) - 1
];

% Valeurs initiales pour [x, y, z]
initial_guess = [0.5, 0.5, 0.5];

% Résolution avec fsolve
solution = fsolve(equations, initial_guess);

% Résultat
x = solution(1);
y = solution(2);
z = solution(3);

% Afficher la solution
disp(['Solution trouvée : x = ', num2str(x), ', y = ', num2str(y), ', z = ', num2str(z)]);

% ====== ADXL345
% Solution trouvée : x = 0.0040005, y = 0.0039862, z = 0.0038718
% ====== ADXL359
% Solution trouvée : x = 3.9673e-05, y = 3.6318e-05, z = 4.0804e-05




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
    xlabel('Valeur (en g +- 20g)');
    ylabel('Densité');
    legend('Histogramme des données', 'Loi normale');
    hold off;  % Fin du maintien de la figure

end




