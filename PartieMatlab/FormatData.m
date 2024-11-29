nomFichier = "NouvelData.txt";
nomFichierFormate = strcat(nomFichier, "_formate.txt");

% Ouvrir le fichier en lecture
fid = fopen(nomFichier, 'r');
% Ouvrir un nouveau fichier pour écrire le résultat
fid_out = fopen(nomFichierFormate, 'w');

% Lire le fichier ligne par ligne
while ~feof(fid)
    line = fgetl(fid); % Lire une ligne
    % Trouver la première espace (indique la fin du timestamp)
    space_index = strfind(line, ' '); 
    if ~isempty(space_index)
        % Extraire la partie après le timestamp
        new_line = line(space_index(1)+1:end); % +1 pour ignorer l'espace
        % Écrire la nouvelle ligne dans le fichier de sortie
        fprintf(fid_out, '%s\n', new_line);
    end
end

% Fermer les fichiers
fclose(fid);
fclose(fid_out);

disp('Transformation terminée!');
