#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int found(const char *file, const char *username, const char *password);

int main()
{
    printf("Content-type: text/html\n\n");
    
    char username[50] = {0}, password[50] = {0};
    
    int content_length = atoi(getenv("CONTENT_LENGTH"));
    
    if (content_length > 0) {
        char *post_data = malloc(content_length + 1);
        if (post_data == NULL) {
            printf("<p>Erreur : échec de l'allocation de mémoire.</p>");
            return 1;
        }
        
        fread(post_data, 1, content_length, stdin);
        post_data[content_length] = '\0'; 
        
        sscanf(post_data, "username=%49[^&]&password=%49s", username, password);
        free(post_data);
    } else {
        printf("<p>Erreur : aucune donnée trouvée dans CONTENT_LENGTH.</p>");
        return 1;
    }

    if (found("users.txt", username, password)) {
        printf("<meta http-equiv=\"refresh\" content=\"0;url=/welcome.html\" />");
    } else {
        printf("<!DOCTYPE html>");
        printf("<html lang=\"fr\">");
        printf("<head>");
        printf("<meta charset=\"UTF-8\">");
        printf("<title>Erreur de Connexion</title>");
        printf("</head>");
        printf("<body>");
        printf("<h2>Formulaire de Connexion</h2>");
        printf("<form action=\"verify_login_post.cgi\" method=\"post\">");
        printf("<label for=\"username\">Nom d'utilisateur:</label>");
        printf("<input type=\"text\" id=\"username\" name=\"username\" value=\"%s\" required><br><br>", username);
        printf("<label for=\"password\">Mot de passe:</label>");
        printf("<input type=\"password\" id=\"password\" name=\"password\" value=\"%s\" required><br><br>", password);
        printf("<input type=\"submit\" value=\"Se connecter\">");
        printf("</form>");
        printf("</body>");
        printf("</html>");
    }

    return 0;
}

int found(const char *file, const char *username, const char *password) {
    FILE *file_ptr = fopen(file, "r");
    if (file_ptr == NULL) {
        printf("<p>Erreur lors de l'ouverture du fichier des utilisateurs.</p>");
        return 0;
    }

    char file_username[50], file_password[50];
    while (fscanf(file_ptr, "%49s %49s", file_username, file_password) != EOF) {
        if (strcmp(username, file_username) == 0 && strcmp(password, file_password) == 0) {
            fclose(file_ptr);
            return 1;
        }
    }

    fclose(file_ptr);
    return 0;
}
