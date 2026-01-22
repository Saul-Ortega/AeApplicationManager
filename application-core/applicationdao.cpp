#include "applicationdao.h"

#include <QString>
#include <QList>
#include <QFile>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include "version.h"

// === constructor ===
ApplicationDao::ApplicationDao(const QString& filePath) :
    mFilePath("C://Users//AlejandroRodriguez//Desktop//RepositoriosGit//AeApplicationManager//application-core//applications.json")
{
}

// === load all ===
QList<Application> ApplicationDao::loadApplications()
{
    //COGE LA RUTA DESDE RESOURCE.QRC
    QFile jsonFile(":/data/applications.json");

    //COMPRUEBA SI EL ARCHIVO EXISTE
    if ( !jsonFile.exists() ) {
        qDebug() << "No se puede abrir el fichero";
        return QList<Application>();
    }

    //ABRE EL ARCHIVO CON PERMISO DE SÓLO LECTURE
    jsonFile.open(QIODevice::ReadOnly);
    //CARGA TODOS LOS BYTES DEL ARCHIVO
    QByteArray ba = jsonFile.readAll();
    //CERRAMOS EL ARCHIVO DESPUES DE LEERLO
    jsonFile.close();

    //DECLARA UN ERROR PARSER PARA SABER SI EL JSON ESTÁ BIEN FORMADO
    QJsonParseError errorParser;
    //INSTANCIA UN QJSONDOCUMENT DE UN ARCHIVO JSON
    QJsonDocument jsonDocument = QJsonDocument::fromJson(ba, &errorParser);

    //COMPRUEBA QUE EL DOCUMENTO JSON NO TENGA NINGÚN ERROR DE SINTAXIS
    if ( errorParser.error != 0 ) {
        qDebug() << errorParser.errorString();
        return QList<Application>();
    }

    //COMPRUEBA QUE EL QJSONDOCUMENT NO SEA NULO
    if ( jsonDocument.isNull() ) {
        qDebug() << "El archivo está vacío";
        return QList<Application>();
    }

    //COMPRUEBA SI EL QJSONDOCUMENT ES UN ARRAY DE OBJETOS
    if ( jsonDocument.isArray() ) {
        //CONVIERTE EL QJSONDOCUMENT EN UN QJSONARRAY
        QJsonArray jsonArray = jsonDocument.array();

        QList<Application> applications;

        //ITERA SOBRE CADA APLICACIÓN
        for ( auto applicationElement : jsonArray ) {
            //INICIALIZA EL OBJETO
            QJsonObject object = applicationElement.toObject();
            //RECOGE LOS DATOS EN UN QVARIANTMAP QUE PERMITE ALMACENAR DATOS DE DISTINTOS TIPOS
            QVariantMap jsonMap = object.toVariantMap();

            //DECLARA UN OBJETO DE TIPO APPLICATION
            Application application;

            //SETEA LOS DATOS DE LA APLICACIÓN
            application.setId(jsonMap["id"].toInt());
            application.setName(jsonMap["name"].toString());
            application.setDescription(jsonMap["description"].toString());
            application.setImageUrl(jsonMap["image_url"].toString());
            application.setExecutableFile(jsonMap["executable_file"].toString());
            application.setExpirationDate(jsonMap["expiration_date"].toDate());
            application.setIsLiked(jsonMap["is_liked"].toBool());
            application.setIsDownloaded(jsonMap["is_downloaded"].toBool());

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

            application.setVersions(versions);

            //AÑADE LA APLICACIÓN A LA LISTA DE APLICACIONES
            applications.append(application);
        }

        return applications;
    }

    return QList<Application>();
}

// === SAVE ALL ===
void ApplicationDao::saveAll(const QList<Application>& apps)
{
    // array principal donde se guardaran todas las aplicaciones
    QJsonArray jsonArray;

    // recorrer todas las aplicaciones
    for (const Application& app : apps) {

        // un objeto json para cada aplicacion
        QJsonObject appObj;
        appObj["id"] = app.id;
        appObj["name"] = app.name;
        appObj["description"] = app.description;
        appObj["image_url"] = app.imageUrl;
        appObj["executable_file"] = app.executableFile;
        appObj["expiration_date"] = app.expirationDate;
        appObj["is_liked"] = app.isLiked;
        appObj["is_downloaded"] = app.isDownloaded;

        // JsonArray para las versiones de esta aplicacion
        QJsonArray versionsArray;

        // recorrer todas las versiones
        for (const Version& version : app.versions) {

            // un objeto json para una version
            QJsonObject versionObj;
            versionObj["id"] = version.id;
            versionObj["name"] = version.name;
            versionObj["size"] = version.size;
            versionObj["last_modification"] = version.lastModification;
            versionObj["expiration_date"] = version.expirationDate;
            versionObj["is_installed"] = version.is_installed;

            // añadir la version al array de versiones
            versionsArray.append(versionObj);
        }

        // añadir el array de versiones al objeto de la aplicacion
        appObj["versions"] = versionsArray;

        // añadir la aplicación al array principal
        jsonArray.append(appObj);
    }

    // convertir el array completo en un documento json
    QJsonDocument document(jsonArray);

    // abrir el archivo para escritura
    QFile jsonFile(mFilePath);
    jsonFile.open(QIODevice::WriteOnly);

    // comprobar si se abrio correctamente
    if (!jsonFile.isOpen()) {
        qDebug() << "no se pudo abrir el archivo para escribir:" << mFilePath;
        return;
    }

    // escribir el json con formato indentado
    jsonFile.write(document.toJson(QJsonDocument::Indented));
    jsonFile.close();

    qDebug() << "archivo guardado correctamente.";
}

