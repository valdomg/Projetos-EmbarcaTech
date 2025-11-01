from MongoDB.MongoDBConnection import MongoDBConnection
from datetime import datetime, timezone

class ChamadasDBModel():
    
    def __init__(self, db:MongoDBConnection):
        self.db = db

    def return_all_chamadas(self):
        return self.db.list_all_documents_from_collection('chamadas')
    
    def return_chamadas_by_day(self):
        date_now = datetime.now()
        start_date = date_now.replace(hour=0, minute=00, second=00, microsecond=0)
        end_date = date_now.replace(hour=23, minute=59, second=00, microsecond=9999)
        
        return self.db.list_documents_by_date('chamadas', 'data', start_date, end_date)

    def return_number_of_chamadas_by_day(self):
        date_now = datetime.now()
        start_date = date_now.replace(hour=0, minute=00, second=00, microsecond=0)
        end_date = date_now.replace(hour=23, minute=59, second=00, microsecond=9999)
    
        return self.db.count_documents_by_date('chamadas', 'data', start_date, end_date)
    
    def return_chamadas_by_month(self, month:int):
        date_now = datetime.now()
        start_date = date_now.replace(day=1, month=month, minute=00, second=00, microsecond=0)
        end_date = date_now.replace(day=31, month=month, minute=59, second=00, microsecond=9999)

        return self.db.count_documents_by_date('chamadas', 'data', start_date, end_date)

    def return_number_of_chamadas_by_month(self, month:int):
        date_now = datetime.now()
        start_date = date_now.replace(day=1, month=month, hour=0, minute=00, second=00, microsecond=0)
        end_date = date_now.replace(day=31, month=month, hour=23, minute=59, second=00, microsecond=9999)
    
        return self.db.count_documents_by_date('chamadas', 'data', start_date, end_date)

    def return_count_all_chamadas(self):
        return self.db.count_all_documents_on_collection('chamadas')