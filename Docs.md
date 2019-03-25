
		// 405 	-> Si METHOD Connue mais non admise
		// 501	-> Si METHOD Non connue

		// Host -> Sert à determiner un serveur d'hote virtuel (5.2)
			// Voir avec le groupe le 5.2

		// 13.5 	-> Reconstruction a partir du cache

GENERAL-HEADERS
	Cache-Control       14.9
✓	Connection          14.10
✓	Date                14.18
	Pragma              14.32
✗	Trailer             14.40
✓	Transfer-Encoding   14.41
	Upgrade             14.42
	Via                 14.45
	Warning             14.46

ENTITY-HEADERS
✓	Allow               14.7
✓	Content-Encoding    14.11
✗	Content-Language    14.12
✓	Content-Length      14.13
✓	Content-Location    14.14
✓	Content-MD5         14.15
✗	Content-Range       14.16
✓	Content-Type        14.17
✓	Expires             14.21
✓	Last-Modified       14.29

REQUEST-HEADERS
✓	Accept              14.1
✓	Accept-Charset      14.2
✓	Accept-Encoding     14.3
✗	Accept-Language     14.4
✗	Authorization       14.8
✗	Expect              14.20
✓	From                14.22
✓	Host                14.23
✓	If-Match            14.24
✓	If-Modified-Since   14.25
✓	If-None-Match       14.26
✓	If-Range            14.27
✓	If-Unmodified-Since 14.28
✗	Max-Forwards        14.31
✗	Proxy-Authorization 14.34
✗	Range               14.35
✗	Referring           14.36
✗	TE                  14.39
✓	User-Agent          14.43

RESPONSE-HEADERS
✗	Accept-Ranges       14.5
✓	Age                 14.6
✓	ETag                14.19
✗	Rental              14.30
✗	Proxy-Authenticate  14.33
✗	Retry-After         14.37
✓	Server              14.38
✗	Vary                14.44
✗	WWW-Authenticate    14.47


14.1 :: Accept
    Definit les types de supports acceptables
        Tools:
            - WIN -> http://stackoverflow.com/questions/20634666/get-a-mime-type-from-a-extension-in-c
            - UNIX -> http://stackoverflow.com/questions/423626/get-mime-type-from-filename-in-c
    NON OBLIGATOIRE    
    Retours:
        Erreur: 406

14.2 :: Accept-Charset
    Definits les charsets
        Liste des charsets:
            - http://www.iana.org/assignments/character-sets/character-sets.xhtml
        Tools:
			- WIN/UNIX -> https://github.com/AutoIt/text-encoding-detect
    NON OBLIGATOIRE    
    Retours:
        Erreur: 406

14.3 :: Accept-Encoding
    Gestion de l'encoding (cf. ContentEncoding)
    NON OBLIGATOIRE    
    Retours:
        Erreur: 406

14.4 :: Accept-Language
    Gestion du langage (3.10)
    IGNORE

14.5 :: Accept-Ranges
    Gestion des gammes d'octet (cf 3.12)
        Desactivation des ranges
            "none"
        Sinon
            "bytes"
    NON OBLIGATOIRE

14.6 :: Age
    Age de la reponse (Attention, verification d'overflow) (cf 13.2.3)
        Attention: Overflow -> retourner 2^31
    UTILISABLE QUAND IL Y A DU CACHE
        

14.7 :: Allow
    Methodes autorisées
    SEULEMENT EN CAS DE REPONSE A UN 405
    

14.8 :: Authorization
    Permet la connection
    Format
        Basic (Username:Password | base64)
    SEULEMENT LORS D'UNE CONNECTION

14.9 :: Cache-Control
    Gestion du cache
        Trop complet (priorité mini)
    LORS D'UNE GESTION DE CACHE

14.10 :: Connection
    Permet à l'envoyeur de spécifier des options désirées pour cette connexion particulière
    ATTENTION, ONLY 'CLOSE' CAN CLOSE CONNECTION
    ATTENTION, pour les versions < HTTP 1.0, Connection contient les Entêtes à ne pas prendre en compte

14.11 :: Content-Encoding:
    Gestion des encodages
    Liste des encodages:
        gzip
        compress
        deflate
        indenty

14.12 :: Content-Language
    IGNORE

14.13 :: Content-Length
    Taille du entity body
        Faire la size du entity body après tout traitement
    OBLIGATOIRE

14.14 :: Content-Location
    Addresse de la ressource sur le serv
    A REVOIR
    

14.15 :: Content-MD5
    Footprint du entity body
    Attention, seulement après encodage du contenu et avant encodage de trasfert

14.16 :: Content-Range
    Donne des informations concernant les chuncks
    Syntaxe:
        START-END/TOTAL

        Variables:
            START   = Debut de la range
            END     = Fin de la range
            TOTAL   = Taille du contenu total
        Conditions:
            Si erreur 416:
                START-END = '*'
            Si taille du contenu inconnue
                TOTAL = '*'
        
        Si erreur 416 et si la longueur du contenu est inconnue:
            bytes */*
        Si erreur 416
            bytes */TOTAL
        Si la longueur du contenu est inconnue
            bytes START-END/*
        En tant normal
            bytes START-END/TOTAL
        

14.17 :: Content-Type
    Type de contenu
        Retourne le type de contenu (cf 14.1)
    OBLIGATOIRE SI IL Y A UN ENITY

14.18 :: Date
    Date d'origine du message
        OBLIGATOIRE SAUF:
            Error 100 / 101 (non obligatoire)
            Error 500 / 503
    
14.19 :: ETag
    A revoir (cf. 14.24, 14.26, 14.44 + 13.3.3)

14.20 :: Expect
    Filtre de se qu'attends le client
    SI INCOMPREHENSIBLE, RETOURNER 417
    
14.21 :: Expires
    Defini quand la réponse est périmé
        Quelques points importants
            Si dans le Cache-Control, il y a la directive "max-age" or "s-max-age", "Expires" est ignoré
            Si le format est invalide (différent de la RFC 1123), la réponse est dite expirée
            Pour marquer une réponse "déjà expiré", le serveur renvoi un "Expires" équal à "Date"
            Pour marquer une réponse "jamais expiré", le serveur renvoi un "Expires" équal à NOW + 1 an

14.22 :: From
    Contient une adresse mail
    IGNORE

14.23 :: Host
    Specifie l'hote et le port
        Ne doit contenir que l'hote + le port (80 par défaut)
        Pour le serveur
            Si la Request-Uri est absolue, Host est ignoré
            Si la Request-Uri est relative, l'hote est determiné par Host
    OBLIGATORE
        Reponse 400

14.24 :: If-Match
    Condition correspondance d'une entité
        On doit utiliser la comparaison forte (13.3.3)
        Si la valeur est '*', alors la méthode peut être exécuté sur toutes entité
        Si la condition est foireuse, ou si la valeur est '*' mais qu'aucune entité n'est trouvé, on retourne 412
        Une demande destinée à mettre à jour une ressource (par exemple, un PUT) PEUT inclure un champ d’en-tête
         If-Match pour signaler que la méthode de demande NE DOIT PAS être appliquée si l'entité correspondant à la
         valeur de If-Match (une seule étiquette d'entité) n’est plus une représentation de cette ressource
        Plusieurs tags sont possible (CSV)

14.25 :: If-Modified-Since
    Condition de validation d'une entité
         Si la variante demandée n’a pas été modifiée depuis le moment spécifié dans ce champ, une entité ne sera pas retournée du serveur (et on retourne 304)
         Attention, utilisation de HTTP-date
         
14.26 :: If-None-Match
    Condition de non correspondance d'une entité
        Verifie que l'entité en question ne correspond a aucun des tags fourni

14.27 :: If-Range

14.28 :: If-Unmodified-Since
    Condition de validation d'une entité
         Si la variante demandée a pas été modifiée depuis le moment spécifié dans ce champ, le serveur ne doit pas executer l'opération demandé (et on retourne 412)
         Attention, utilisation de HTTP-date

14.29 :: Last-Modified
    Donne la date de la dernière modification
        Attention, si Last-Modifier > Date, on retourne Date
    NON OBLIGATOIRE

14.30 :: Location
    Utilisé pour rediriger le receveur sur une localisation autre que l’URI de demande
    Utilisable lors d'un 201 ou d'un 3XX

14.31 :: Max-Forwards
    Limite le nombre de proxy utilisé par TRACE ou OPTION

14.32 :: Pragma
    IGNORE

14.33 :: Proxy-Authenticate
    IGNORE

14.34 :: Proxy-Authorization
    IGNORE

14.35 :: Range
    Permet de récuperrer différente partie d'une entité, sans tout prendre
    NON OBLIGATOIRE, MAIS MIEUX VAUT L'IMPLEMENTER

14.36 :: Referer
    Permet au client de spécifier l’adresse (URI) de la ressource de laquelle l’URI de demande a été obtenu 
    IGNORE, OU VOIR UTILITE AVEC LE CACHE

14.37 :: Retry-After
    Indiquer pendant combien de temps on pense que le service va être indisponible pour le client demandeur
        Peut être utilisé avec 503 ou 3XX
        Utilisable avec HTTP-date ou delta-second

14.38 :: Server
    Information concernant le serveur (nom, comment, ...)
    OBLIGATOIRE

14.39 :: TE
    Indique quels codages d’extension de transfert il acceptera dans la réponse et si il veut ou non accepter des champs de queue dans un codage de transfert fragmenté
        Le champ d’en-tête TE ne s’applique qu’à la connexion immédiate
        La présence du mot clé "trailers" indique que le client est d’accord pour accepter les champs d'en-queue (cf. Trailer)
    NON OBLIGATOIRE

14.40 :: Trailer
    Donne les headers qui seront disponible dans la queue du message
        OBLIGATOIRE LORS D'UNE TRANSFERT-ENCODING A CHUNCKED
        Attention, les champs "Transfer-Encoding", "Trailer" et "Content-Length" ne doivent pas apparaitre dans le Trailer

14.41 :: Transfer-Encoding
    Encodage de transfert
        Si l'encodage n'est pas défini, on retourne 501
        Encodage possible
            chuncked
                Donnée en série de chunk. cf https://fr.wikipedia.org/wiki/Chunked_transfer_encoding
            compress
                LWZ algorithm
            deflate
                zlib deflate algrotithm
            gzip
                LZ77 algorithm
            identity
                Aucune modification
    OBLIGATOIRE EN CAS D'ENCODAGE DE TRANSFERT

14.42 :: Upgrade
    Donne la liste des protocoles de communication supporté
    OBLIGATOIRE LORS D'UN 101

14.43 :: User-Agent
    User-Agent du client
    NON OBLIGATOIRE (DEVRAIENT)

14.44 :: Vary

14.45 :: Via
    Chemin utilisé par les proxy
        Exemple: 1.0 fred, 1.1 nowhere.com (Apache/1.1)
    IGNORE

14.46 :: Warning
    Donne des infos supplémentaires concernant l'état ou la transformation du message
    TODO: Faire la liste des types de WARNING

14.47 :: WWW-Authenticate
    Donne les challenges pour l'authentification durant un retour 401
    https://fr.wikipedia.org/wiki/Authentification_HTTP
