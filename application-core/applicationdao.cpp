#include "applicationdao.h"

#include <QString>
#include <QList>
#include <QFile>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include "Version.h"

//CONSTRUCTOR
ApplicationDao::ApplicationDao()
{
}

//ACTUALIZA UNA APLICACIÓN
void ApplicationDao::updateApplication(const Application& application) const
{
    //COGE LA RUTA DESDE RESOURCE.QRC
    QFile jsonFile("../../../../application-core/applications.json");

    // ABRIR JSON SOLO LECTURA
    if (!jsonFile.open(QIODevice::ReadOnly)) {
        qDebug() << "No se pudo abrir el archivo para leer: ";
        return;
    }

    //CARGA TODOS LOS BYTES DEL ARCHIVO
    QByteArray ByteArray = jsonFile.readAll();
    //CERRAMOS EL ARCHIVO DESPUES DE LEERLO
    jsonFile.close();

    //DECLARA UN ERROR PARSER PARA SABER SI EL JSON ESTÁ BIEN FORMADO
    QJsonParseError errorParser;
    //INSTANCIA UN QJSONDOCUMENT DE UN ARCHIVO JSON
    QJsonDocument jsonDocument = QJsonDocument::fromJson(ByteArray, &errorParser);

    //COMPRUEBA QUE EL DOCUMENTO JSON NO TENGA NINGÚN ERROR DE SINTAXIS
    if (errorParser.error != 0 || !jsonDocument.isArray()) {
        qDebug() << "Error al parsear JSON existente";
        return;
    }

    //CONVIERTE EL QJSONDOCUMENT EN UN QJSONARRAY PARA LAS APPS
    QJsonArray jsonArray = jsonDocument.array();


    //DECLARA UN QJSONOBJECT
    QJsonObject appObj;
    appObj["id"] = application.id();
    appObj["name"] = application.name();
    appObj["description"] = application.description();
    appObj["image_url"] = application.imageUrl();
    appObj["executable_file"] = application.executableFile();
    appObj["expiration_date"] = application.expirationDate().toString("yyyy-MM-dd");
    appObj["is_liked"] = application.isLiked();
    appObj["is_downloaded"] = application.isDownloaded();

    //DECLARA UN QJSONARRAY PARA LAS GUARDAR LAS VERSIONES DE LA APLICACIÓN
    QJsonArray versionsArray;

    //ITERA SOBRE CADA VERSIÓN
    for (const Version& version : application.versions()) {
        //DECLARA UN QJSONOBJECT PARA UNA VERSIÓN
        QJsonObject versionObj;
        versionObj["id"] = version.id();
        versionObj["name"] = version.name();
        versionObj["size"] = version.size();
        versionObj["last_modification"] = version.lastModification().toString("yyyy-MM-dd");
        versionObj["expiration_date"] = version.expirationDate().toString("yyyy-MM-dd");
        versionObj["is_installed"] = version.isInstalled();

        //AÑADIR LA VERSIÓN AL ARRAY DE VERSIONES
        versionsArray.append(versionObj);
    }

    //AÑADIR EL ARRAY DE VERSIONES AL OBJETO DE LA APLICACIÓN
    appObj["versions"] = versionsArray;

    //BUSCA QUE APLICACION TIENE EL MISMO ID PARA ACTUALIZAR
    for (int i = 0; i < jsonArray.size(); ++i) {
        QJsonObject app = jsonArray[i].toObject();

        //SI EL OBJ TIENE EL MISMO ID QUE EL QUE LE PASARON LO ACTUALIZA
        if (app["id"].toInt() == application.id()) {
            jsonArray[i] = appObj;
            break;
        }
    }


    //CONVERTIR EL ARRAY COMPLETO EN UN QJSONDOCUMENT
    QJsonDocument document(jsonArray);

    //ABRIR EL ARCHIVO PARA ESCRITURA
    jsonFile.open(QIODevice::WriteOnly);

    //COMPROBAR SI SE ABRIÓ CORRECTAMENTE
    if (!jsonFile.isOpen()) {
        qDebug() << "No se pudo abrir el archivo para escribir: ";
        return;
    }

    //ESCRIBIR EL JSON CON FORMATO IDENTADO
    jsonFile.write(document.toJson(QJsonDocument::Indented));
    //CIERRA EL ARCHIVO JSON
    jsonFile.close();

    qDebug() << "Archivo guardado correctamente.";
}

//CARGA TODAS LAS APLICACIONES DEL JSON
std::unique_ptr<std::vector<std::unique_ptr<Application>>> ApplicationDao::applications() const
{
    //COGE LA RUTA DESDE RESOURCE.QRC
    QFile jsonPath("../../../../application-core/applications.json");

    //COMPRUEBA SI EL ARCHIVO EXISTE
    if ( !jsonPath.exists() ) {
        qDebug() << "No se puede abrir el fichero";
        return std::unique_ptr<std::vector<std::unique_ptr<Application>>>();
    }

    //ABRE EL ARCHIVO CON PERMISO DE SÓLO LECTURE
    jsonPath.open(QIODevice::ReadOnly);
    //CARGA TODOS LOS BYTES DEL ARCHIVO
    QByteArray ba = jsonPath.readAll();
    //CERRAMOS EL ARCHIVO DESPUES DE LEERLO
    jsonPath.close();

    //DECLARA UN ERROR PARSER PARA SABER SI EL JSON ESTÁ BIEN FORMADO
    QJsonParseError errorParser;
    //INSTANCIA UN QJSONDOCUMENT DE UN ARCHIVO JSON
    QJsonDocument jsonDocument = QJsonDocument::fromJson(ba, &errorParser);

    //COMPRUEBA QUE EL DOCUMENTO JSON NO TENGA NINGÚN ERROR DE SINTAXIS
    if ( errorParser.error != 0 ) {
        qDebug() << errorParser.errorString();
        return std::unique_ptr<std::vector<std::unique_ptr<Application>>>();
    }

    //COMPRUEBA QUE EL QJSONDOCUMENT NO SEA NULO
    if ( jsonDocument.isNull() ) {
        qDebug() << "El archivo está vacío";
        return std::unique_ptr<std::vector<std::unique_ptr<Application>>>();
    }

    //COMPRUEBA SI EL QJSONDOCUMENT ES UN ARRAY DE OBJETOS
    if ( jsonDocument.isArray() ) {
        //CONVIERTE EL QJSONDOCUMENT EN UN QJSONARRAY
        QJsonArray jsonArray = jsonDocument.array();

        std::unique_ptr<std::vector<std::unique_ptr<Application>>> applications(new std::vector<std::unique_ptr<Application>>());

        //ITERA SOBRE CADA APLICACIÓN
        for ( auto applicationElement : jsonArray ) {
            //INICIALIZA EL OBJETO
            QJsonObject object = applicationElement.toObject();
            //RECOGE LOS DATOS EN UN QVARIANTMAP QUE PERMITE ALMACENAR DATOS DE DISTINTOS TIPOS
            QVariantMap jsonMap = object.toVariantMap();

            //DECLARA UN PUNTERO DE TIPO APPLICATION
            std::unique_ptr<Application> application(new Application());

            //SETEA LOS DATOS DE LA APLICACIÓN
            application->setId(jsonMap["id"].toInt());
            application->setName(jsonMap["name"].toString());
            application->setDescription(jsonMap["description"].toString());
            application->setImageUrl(jsonMap["image_url"].toString());
            application->setExecutableFile(jsonMap["executable_file"].toString());
            application->setExpirationDate(jsonMap["expiration_date"].toDate());
            application->setIsLiked(jsonMap["is_liked"].toBool());
            application->setIsDownloaded(jsonMap["is_downloaded"].toBool());

            //DECLARA UNA LISTA DE TIPO VERSION
            QList<Version> versions;

            //SE ALMACENA EN UN QJSONARRAY TODAS LAS VERSIONES CORRESPONDIENTES A UNA APP
            QJsonArray versionsJsonArray = jsonMap["versions"].toJsonArray();

            //ITERA SOBRE CADA VERSIÓN
            for ( auto versionElement : versionsJsonArray ) {
                //INICIALIZA EL OBJETO
                QJsonObject versionObject = versionElement.toObject();
                //RECOGE LOS DATOS EN UN QVARIANTMAP QUE PERMITE ALMACENAR DATOS DE DISTINTOS TIPOS
                QVariantMap versionMap = versionObject.toVariantMap();

                //DECLARA UN OBJETO DE TIPO VERSION
                Version version;

                //SETEA LOS DATOS DE TIPO VERSION
                version.setId(versionMap["id"].toInt());
                version.setName(versionMap["name"].toString());
                version.setLastModification(versionMap["last_modification"].toDate());
                version.setExpirationDate(versionMap["expiration_date"].toDate());
                version.setIsInstalled(versionMap["is_installed"].toBool());

                //AÑADE LA VERSION A LA LISTA DE VERSIONES
                versions.append(version);
            }

            application->setVersions(versions);

            //AÑADE LA APLICACIÓN A LA LISTA DE APLICACIONES
            applications->push_back(std::move(application));
        }

        return applications;
    }

    return std::unique_ptr<std::vector<std::unique_ptr<Application>>>();
}
