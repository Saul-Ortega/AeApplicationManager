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

// === CONSTRUCTOR ===
ApplicationDao::ApplicationDao(){}

// === LOAD ALL ===
QList<Application> ApplicationDao::loadApplications()
{
    //COGE LA RUTA DESDE RESOURCE.QRC
    QFile jsonPath(":/data/applications.json");

    //COMPRUEBA SI EL ARCHIVO EXISTE
    if ( !jsonPath.exists() ) {
        qDebug() << "No se puede abrir el fichero";
        return QList<Application>();
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
    //COGE LA RUTA DESDE RESOURCE.QRC
    QFile jsonPath("../../application-core/applications.json");


    //COMPRUEBA SI EL ARCHIVO EXISTE
    if ( !jsonPath.exists() ) {
        qDebug() << "El archivo JSON no existe";
        //AUNQUE NO EXISTA, LO CREAMOS IGUAL
        //NO HACEMOS RETURN PARA PERMITIR CREARLO
    }

    //ABRE EL ARCHIVO CON PERMISO DE ESCRITURA
    if ( !jsonPath.open(QIODevice::WriteOnly) ) {
        qDebug() << "No se pudo abrir el archivo JSON para escribir";
        return;
    }

    //DECLARA UN QJSON ARRAY PRINCIPAL PARA TODAS LAS APLICACIONES
    QJsonArray jsonArray;

    //ITERA SOBRE TODAS LAS APLICACIONES
    for (const Application& app : apps) {

        //CREA UN OBJETO JSON PARA UNA APLICACIÓN
        QJsonObject appObj;
        appObj["id"] = app.id();
        appObj["name"] = app.name();
        appObj["description"] = app.description();
        appObj["image_url"] = app.imageUrl();
        appObj["executable_file"] = app.executableFile();
        appObj["expiration_date"] = app.expirationDate().toString("yyyy-MM-dd");
        appObj["is_liked"] = app.isLiked();
        appObj["is_downloaded"] = app.isDownloaded();

        //CREA UN ARRAY JSON PARA LAS VERSIONES DE ESTA APLICACIÓN
        QJsonArray versionsArray;

        //ITERA SOBRE TODAS LAS VERSIONES
        for (const Version& version : app.versions()) {

            //CREA UN OBJETO JSON PARA UNA VERSIÓN
            QJsonObject versionObj;
            versionObj["id"] = version.id();
            versionObj["name"] = version.name();
            versionObj["size"] = version.size();
            versionObj["last_modification"] = version.lastModification().toString("yyyy-MM-dd");
            versionObj["expiration_date"] = version.expirationDate().toString("yyyy-MM-dd");
            versionObj["is_installed"] = version.isInstalled();

            //AÑADE LA VERSIÓN AL ARRAY DE VERSIONES
            versionsArray.append(versionObj);
        }

        //AÑADE EL ARRAY DE VERSIONES AL OBJETO DE LA APLICACIÓN
        appObj["versions"] = versionsArray;

        //AÑADE LA APLICACIÓN AL ARRAY PRINCIPAL
        jsonArray.append(appObj);
    }

    //CONVIERTE EL ARRAY COMPLETO EN UN DOCUMENTO JSON
    QJsonDocument document(jsonArray);

    //ESCRIBE EL JSON EN EL ARCHIVO
    jsonPath.write(document.toJson(QJsonDocument::Indented));

    //CIERRA EL ARCHIVO
    jsonPath.close();

    qDebug() << "Archivo guardado correctamente.";
}

