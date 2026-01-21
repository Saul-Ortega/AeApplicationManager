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
QList<Application> ApplicationDao::loadApplication() const
{
    QList<Application> list;              // lista donde se guardarán las aplicaciones
    QFile file(mFilePath);                // archivo donde está el json

    // intentar abrir el archivo
    file.open(QIODevice::ReadOnly);

    // verificar si se abrió correctamente
    if (!file.isOpen()) {
        qDebug() << "no se pudo abrir el archivo json:" << mFilePath;
        return list;
    }

    // leer todo el contenido del archivo
    QByteArray ba = file.readAll();
    file.close();

    // parser de errores para validar el json
    QJsonParseError errorParser;

    // crear un documento json a partir del contenido
    QJsonDocument jsonDocument = QJsonDocument::fromJson(ba, &errorParser);

    // comprobar si hubo errores de sintaxis
    if (errorParser.error != QJsonParseError::NoError) {
        qDebug() << "error al parsear json:" << errorParser.errorString();
        return list;
    }

    // comprobar si el documento está vacío
    if (jsonDocument.isNull()) {
        qDebug() << "el archivo json está vacío";
        return list;
    }

    // comprobar si el documento es un array
    if (!jsonDocument.isArray()) {
        qDebug() << "el json no es un array";
        return list;
    }

    // convertir el documento en un array
    QJsonArray jsonArray = jsonDocument.array();

    // recorrer cada elemento del array
    for (const QJsonValue& value : jsonArray) {
        //FALTA
    }

    return list;
}

// === save all ===
void ApplicationDao::saveAll(const QList<Application>& apps)
{
    QJsonArray json_array;

    // recorrer todas las aplicaciones
    for (const Application& app : apps) {

        QJsonObject json_obj;
        json_obj["id"] = app.id;
        json_obj["name"] = app.name;
        json_obj["description"] = app.description;
        json_obj["image_url"] = app.imageUrl;
        json_obj["executable_file"] = app.executableFile;
        json_obj["expiration_date"] = app.expirationDate;
        json_obj["is_liked"] = app.isLiked;
        json_obj["is_downloaded"] = app.isDownloaded;
        json_obj["id_version_installed"] = app.idVersionInstalled;

        // array para las versiones
        QJsonArray versionsArray;

        for (const Version& version : app.versions) {
            QJsonObject versionObj;
            versionObj["id"] = version.id;
            versionObj["name"] = version.name;
            versionObj["size"] = version.size;
            versionObj["last_modification"] = version.lastModification;
            versionObj["expiration_date"] = version.expirationDate;

            versionsArray.append(versionObj);
        }

        json_obj["versions"] = versionsArray;

        // añadir la aplicación al array principal
        json_array.append(json_obj);
    }

    // convertir el array en un documento json
    QJsonDocument document(json_array);

    // abrir el archivo para escritura
    QFile jsonFile(mFilePath);
    jsonFile.open(QIODevice::WriteOnly);

    if (!jsonFile.isOpen()) {
        qDebug() << "no se pudo abrir el archivo para escribir:" << mFilePath;
        return;
    }

    // escribir el json
    jsonFile.write(document.toJson(QJsonDocument::Indented));
    jsonFile.close();

    qDebug() << "archivo guardado correctamente.";
}
