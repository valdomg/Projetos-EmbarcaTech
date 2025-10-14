from Mqtt.application.models.MongoDBConnection import MongoDBConnection


class ChamadasDBModel():
    
    def __init__(self, db:MongoDBConnection):
        self.db = db

    def return_all_chamadas(self):
        return self.db.list_all_documents()
    
    def return_chamadas_by_day(self):
        pass
    
    def return_count_chamadas_by_day(self):
        pass

    def return_count_all_chamadas(self):
        pass