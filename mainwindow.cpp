#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    resetExisting();
    clearPreferences();
    resetChoreEditor();
    ui->ApplicationPages->setCurrentIndex(0);

    QList<QPushButton*> allButtons = ui->CentralWidget->findChildren<QPushButton*>();

    for (QPushButton* button : allButtons) {
        connect(button, &QPushButton::clicked, this, [button] {
            button->resize(button->width() + 2, button->height() + 2);
            QTimer::singleShot(200, button, [&] {
                button->resize(button->width() - 2, button->height() - 2);
            });

        });
    }

    // Log In, Sign Up, Go Back, Log Out
    connect(ui->SignUpButton, &QPushButton::clicked, this, [this] { MainWindow::goToPage(2);});
    connect(ui->LogInButton, &QPushButton::clicked, this, [this] { MainWindow::goToPage(3);});
    connect(ui->LogOutButton, &QPushButton::clicked, this, [this] { MainWindow::goToPage(0);});
    connect(ui->BackButton, &QPushButton::clicked, this, [this] { MainWindow::goToPage(1);});

    // Submit Login/SignUp Information
    connect(ui->SubmitLogin, &QPushButton::clicked, this, &MainWindow::submitLoginInfo);
    connect(ui->SubmitSignUp, &QPushButton::clicked, this, &MainWindow::submitSignUpInfo);
    connect(ui->SubmitLogin, &QPushButton::clicked, this, &MainWindow::resetExisting);
    connect(ui->SubmitSignUp, &QPushButton::clicked, this, &MainWindow::resetExisting);

    // Add Plan
    connect(ui->AddPlanButton, &QPushButton::clicked, this, [this] { MainWindow::goToPage(5);});

    // Connect Community Select Button with Preference
    connect(ui->PlazaVerdeButton, &QPushButton::clicked, this, [this] { MainWindow::getCommunityPreferences(0);});
    connect(ui->PlazaVerde2Button, &QPushButton::clicked, this, [this] { MainWindow::getCommunityPreferences(1);});
    connect(ui->VDCButton, &QPushButton::clicked, this, [this] { MainWindow::getCommunityPreferences(2);});
    connect(ui->VDCNButton, &QPushButton::clicked, this, [this] { MainWindow::getCommunityPreferences(3);});
    connect(ui->CaminoButton, &QPushButton::clicked, this, [this] { MainWindow::getCommunityPreferences(4);});
    connect(ui->PuertaButton, &QPushButton::clicked, this, [this] { MainWindow::getCommunityPreferences(5);});

    connect(ui->SubmitFloorPlan, &QPushButton::clicked, this, &MainWindow::viewFloorPlan);

    connect(ui->SubmitChoreButton, &QPushButton::clicked, this, &MainWindow::addChore);
    connect(ui->AddRoommateButton, &QPushButton::clicked, this, &MainWindow::addRoommate);
    connect(ui->MarkCompleteButton, &QPushButton::clicked, this, &MainWindow::completeChore);
}













// Moving Between Applicating Pages
void MainWindow::goToPage(int pageNumber) {
    if (pageNumber == 4) { // page that shows user's existing plans
        numberOfExistingPlans = getNumberOfDirectories();
        if (!filledIn) {
            fillInExisting();
        }

    }
    // if (pageNumber == 6) {
    //     resetChoreEditor();
    // }
    ui->ApplicationPages->setCurrentIndex(pageNumber);
}

















// Signing In and Out
void MainWindow::submitSignUpInfo() {
    QString homeDir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QString submittedUsername = ui->UsernameEdit_2->text();
    QString newDirPath = homeDir + "/personalTest/" + submittedUsername;

    QDir newDir(newDirPath);

    if (newDir.exists()) {
        qDebug() << "ERROR: Username already taken.";
        goToPage(0);
        return;
    } else {
        if (QDir().mkpath(newDirPath)) {
            QString newPasswordPath = newDirPath + "/secret";
            if (QDir().mkpath(newPasswordPath)) {
                QFile pwFile(newPasswordPath + "/pw.txt");

                if(!pwFile.open(QFile::WriteOnly | QFile::Truncate)) {
                    qDebug() << "ERROR: Can't write to password file.";
                    goToPage(0);
                    return;
                }

                QTextStream out(&pwFile);
                QString submittedPassword = ui->PasswordEdit_2->text();
                out << submittedPassword;
                pwFile.close();
            }

            MainWindow::username = submittedUsername;
            ui->UsernameLabel->setText(submittedUsername);

            qDebug() << "SUCCESS: New User Directory Created: " << newDirPath;
            goToPage(4);
            return;

        }
    }
}

void MainWindow::submitLoginInfo() {
    QString homeDir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QString submittedUsername = ui->UsernameEdit->text();
    QString newDirPath = homeDir + "/personalTest/" + submittedUsername;

    QDir newDir(newDirPath);

    if (!newDir.exists()) {
        qDebug() << "ERROR: Username not in file.";
        goToPage(0);
        return;
    }

    QFile passwordFile(newDirPath + "/secret/pw.txt");
    QString submittedPassword = ui->PasswordEdit->text();

    if (!passwordFile.open(QIODevice::ReadOnly | QFile::Text)) {
        qDebug() << "ERROR: Can't read from password file.";
        goToPage(0);
        return;
    }

    QTextStream in(&passwordFile);
    QString originalPassword = in.readLine();

    if (submittedPassword == originalPassword) {
        MainWindow::username = submittedUsername;
        ui->UsernameLabel->setText(submittedUsername);
        qDebug() << "SUCCESS: " << username << " logged in.";
        goToPage(4);
        return;
    } else {
        qDebug() << "ERROR: Password does not match what's on record.";
        goToPage(0);
        return;
    }
}


















// Load in Existing Plans
void MainWindow::fillInExisting() {
    for (int i = 1; i <= numberOfExistingPlans; i++) {
        addLoadButton(i);
    }
    filledIn = true;
}















// Add New Plan with New Preferences

void MainWindow::getCommunityPreferences(int community) {
    FloorPlan newPlan(nullptr, community);
    myFloorPlan = newPlan;
    ui->FloorPlanDropDown->addItems(myFloorPlan.communityFloorPlans[myFloorPlan.communityIndex]);
    goToPage(6);
}

void MainWindow::addNewPlanDirectory() {
    QString homeDir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QString newDirPath = homeDir + "/personalTest/" + username + "/floorPlan" + QString::number(numberOfExistingPlans + 1);
    numberOfExistingPlans += 1;

    if (QDir().mkpath(newDirPath)) {
        addLoadButton(numberOfExistingPlans);
        return;
    }
}

int MainWindow::getNumberOfDirectories() {
    QString homeDir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QDir newDir(homeDir + "/personalTest/" + username);
    QStringList directories = newDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    return directories.count() - 1;
}


void MainWindow::addLoadButton(int i) {
    QPushButton* loadButton = new QPushButton(ui->ExistingPlanScrollArea);
    loadButton->setGeometry(currentLoadButtonX, currentLoadButtonY, 20, 20);
    loadButton->setText("PLAN" + QString::number(i));
    loadButton->show();

    // set next loadButton location
    if (currentLoadButtonX < 500) {
        currentLoadButtonX += 50;
    } else {
        currentLoadButtonX = 0;
        currentLoadButtonY += 50;
    }

    connect(loadButton, &QPushButton::clicked, this, [loadButton, this] {MainWindow::loadPlan(loadButton);});
}


void MainWindow::viewFloorPlan() {
    int selectedPlanIndex = ui->FloorPlanDropDown->currentIndex();
    myFloorPlan.floorPlanIndex = selectedPlanIndex;
    myFloorPlan.floorPlanName = myFloorPlan.communityFloorPlans[myFloorPlan.communityIndex][myFloorPlan.floorPlanIndex];

    ui->ApartmentDisplay->setTitle(myFloorPlan.floorPlanName);

    QString imageURL = myFloorPlan.getImageURL();
    QString newStyleSheet = QString("border-image: %1, 0 0 0 0 stretch stretch;").arg(imageURL);
    ui->ApartmentDisplay->setStyleSheet(newStyleSheet);

    addNewPlanDirectory();
    clearPreferences();

    goToPage(7);
}













// Detect and Add New Chore
void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (ui->ApartmentDisplay->underMouse()) {
        resetChoreEditor();
        if (event->button() == Qt::LeftButton) {
            QPoint mousePos = event->pos();
            mousePos.setX(mousePos.x() - 145);
            mousePos.setY(mousePos.y() - 170);

            ui->PurpleCircle->show();
            ui->PurpleCircle->move(mousePos);
            ui->ChoreEditor->setCurrentIndex(1);
        }
    } else {
        resetChoreEditor();
    }
}

void MainWindow::bringUpChore(QPushButton* button) {
    ui->ChoreEditor->setCurrentIndex(2);
    ui->PurpleCircle->hide();

    currentChore = button;
    QString location = QString("x%1 y%2").arg(button->x()).arg(button->y());
    int buttonIndex = locationToChore[location];

    ui->ChoreTitle->setText(chores[buttonIndex].title);
    ui->ChoreDescription->setText(chores[buttonIndex].description);
    ui->ChoreAssigned->setText(chores[buttonIndex].assignedTo);
    ui->ChoreDate->setText((chores[buttonIndex].dateAssigned).toString("yyyy-MM-dd"));
}

void MainWindow::addChore() {
    Chore newChore(nullptr);

    newChore.assignedBy = username;
    newChore.assignedTo = ui->RoommateDropDown->currentText();
    newChore.title = ui->ChoreTitleEdit->text();
    newChore.description = ui->ChoreDescEdit->toPlainText();
    newChore.dateAssigned = QDate::currentDate();

    ui->ChoreEditor->setCurrentIndex(2);
    ui->PurpleCircle->hide();

    QPushButton* choreButton = new QPushButton("", ui->ApartmentDisplay);

    choreButton->setGeometry(ui->PurpleCircle->x(), ui->PurpleCircle->y(), 50, 50);
    choreButton->setStyleSheet("border-image: url(:/general/Checklist.png), 0 0 0 0 stretch stretch; background-color: rgba(0,0,0,0);");
    choreButton->show();

    QString location = QString("x%1 y%2").arg(choreButton->x()).arg(choreButton->y());

    newChore.location = choreButton->pos();
    chores.append(newChore);
    locationToChore.insert(location, chores.size() - 1);

    connect(choreButton, &QPushButton::clicked, this, [choreButton, this] {MainWindow::bringUpChore(choreButton);});
}

void MainWindow::completeChore() {
    QString location = QString("x%1 y%2").arg(currentChore->x()).arg(currentChore->y());
    locationToChore.remove(location);
    delete currentChore;
}











// Add Roommate
void MainWindow::addRoommate() {
    QString roommate = QInputDialog::getText(this, "Add Roommate", "Roommate's Name:");
    ui->RoommateDropDown->addItem(roommate);

}











// Reset Preferences

void MainWindow::clearPreferences() {
    // clear away previous chores that were displayed
    for (QPushButton* btn : ui->ApartmentDisplay->findChildren<QPushButton*>()) {
        delete btn;
    }
    locationToChore.clear();

    //clear roommates
    ui->RoommateDropDown->clear();
}

void MainWindow::resetChoreEditor() {
    // hide selector
    ui->PurpleCircle->hide();

    // reset chore editor
    ui->ChoreDescEdit->setPlainText("");
    ui->ChoreDescEdit->setPlaceholderText("enter description here...");
    ui->ChoreTitleEdit->setText("");
    ui->ChoreTitleEdit->setPlaceholderText("enter title here...");
    ui->RoommateDropDown->setCurrentIndex(0);
    ui->ChoreEditor->setCurrentIndex(0);
}

void MainWindow::resetExisting() {
    filledIn = false;

    currentLoadButtonX = 0;
    currentLoadButtonY = 0;

    // remove all loadable plans
    for (QPushButton* btn : ui->ExistingPlanScrollArea->findChildren<QPushButton*>()) {
        delete btn;
    }

    numberOfExistingPlans = getNumberOfDirectories();

    // replace with new user's plans
    if (!filledIn) {
        fillInExisting();
        filledIn = true;
    }
}










// Saving and Loading Plans

void MainWindow::savePlan() {
    QString homeDir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QString userDir = homeDir + "/personalTest/" + username + "/floorPlan" + QString::number(numberOfExistingPlans);
    QFile contents(userDir + "/contents.txt");

    if(!contents.open(QFile::WriteOnly | QFile::Truncate)) {
        qDebug() << "ERROR: Can't write to content file.";
        return;
    }

    QTextStream out(&contents);

    out << "COMMUNITY:" << "\n";
    out << myFloorPlan.communityIndex << "\n";
    out << "\n";

    out << "FLOORPLAN:" << "\n";
    out << myFloorPlan.floorPlanIndex << "\n";
    out << "\n";

    out << "ROOMMATES:" << "\n";
    out << ui->RoommateDropDown->count() << "\n";
    for (int i = 0; i < ui->RoommateDropDown->count(); i++) {
        out << ui->RoommateDropDown->itemText(i) << "\n";
    }
    out << "\n";

    out << "CHORES:" << "\n";
    out << locationToChore.size() << "\n";
    for (const auto& pair : locationToChore.toStdMap()) {
        QString location = pair.first;
        out << location << "\n";

        Chore myChore;
        myChore = chores[pair.second];


        out << myChore.title << "\n";
        out << myChore.description << "\n";
        out << myChore.assignedBy << "\n";
        out << myChore.assignedTo << "\n";

        out << QString::number(myChore.dateAssigned.month()) << "\n";
        out << QString::number(myChore.dateAssigned.day()) << "\n";
        out << QString::number(myChore.dateAssigned.year()) << "\n";

        out << myChore.location.x() << "\n";
        out << myChore.location.y() << "\n";
    }
    out << "\n";

    contents.close();
    return;
}


void MainWindow::loadPlan(QPushButton* button) {
    int index = button->text().toInt();
    QString homeDir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QString userDir = homeDir + "/personalTest/" + username + "/floorPlan" + QString::number(index);
    QFile contents(userDir + "/contents.txt");

    if (!contents.open(QIODevice::ReadOnly | QFile::Text)) {
        qDebug() << "ERROR: Can't write to content file.";
        return;
    }

    QTextStream in(&contents);

    qDebug() << in.readLine(); //COMMUNITY
    myFloorPlan.communityIndex = in.readLine().toInt(); //community number
    qDebug() << in.readLine();

    qDebug() << in.readLine(); //FLOORPLAN
    myFloorPlan.floorPlanIndex = in.readLine().toInt(); //floorplan number
    qDebug() << in.readLine();

    // clear existing chores and roommates
    clearPreferences();

    qDebug() << in.readLine(); //ROOMMATES
    int numOfRoommates = in.readLine().toInt(); //amount of roommates
    for (int i = 0; i < numOfRoommates; i++) {
        QString roommate = in.readLine();
        ui->RoommateDropDown->addItem(roommate);
    }
    qDebug() << in.readLine();

    // read in new chores
    qDebug() << in.readLine(); //CHORES
    int numOfChores = in.readLine().toInt(); //chore amount


    for (int i = 0; i < numOfChores; i++) {
        Chore newChore(nullptr);

        QString btnLocation = in.readLine();

        newChore.title = in.readLine();
        newChore.description = in.readLine();
        newChore.assignedBy = in.readLine();
        newChore.assignedTo = in.readLine();

        int month = in.readLine().toInt();
        int day = in.readLine().toInt();
        int year = in.readLine().toInt();
        newChore.dateAssigned = QDate(year, month, day);

        int x = in.readLine().toInt();
        int y = in.readLine().toInt();
        newChore.location = QPoint(x, y);

        chores.append(newChore);

        QPushButton* choreButton = new QPushButton("", ui->ApartmentDisplay);

        choreButton->setGeometry(x, y, 50, 50);
        choreButton->setStyleSheet("border-image: url(:/general/Checklist.png), 0 0 0 0 stretch stretch; background-color: rgba(0,0,0,0);");
        choreButton->show();

        QString location = QString("x%1 y%2").arg(choreButton->x()).arg(choreButton->y());
        locationToChore.insert(location, chores.size() - 1);

        connect(choreButton, &QPushButton::clicked, this, [choreButton, this] {MainWindow::bringUpChore(choreButton);});
    }

    contents.close();
    goToPage(7);
    return;
}










// Destructor
MainWindow::~MainWindow()
{
    delete ui;
}
