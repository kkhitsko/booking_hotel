/*
* Copyright Khitsko Konstantin 2021-02-22 
*/

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <map>
#include <queue>

using namespace std;

const static int BOOKING_MAX_PERIOD = 86400;


struct Booking {
  int64_t booking_time;
  uint client_id;
  uint rooms_count;
};

class Hotel {
private:
  map< uint, uint> clients_;
  uint rooms_booking_count_;
  queue<Booking> bookings_;

  void removeLastBooking() {
    const Booking& old_booking = bookings_.front();
    rooms_booking_count_ -= old_booking.rooms_count;
    const auto old_client_itr = clients_.find( old_booking.client_id ); //Сложность O(log Q )
    old_client_itr->second -= old_booking.rooms_count;
    /*
     Если у клиента 0 число комнат в списке "активных" бронирований,
     удаляем его из списка
     */
    if ( old_client_itr->second == 0 ) {
      clients_.erase( old_client_itr ); //Сложность O(logQ )
    }

    bookings_.pop();
  }

  void checkBookings( int64_t booking_time ) {
    /*
    Проверяем всю очередь на то, есть ли в ней старые бронирования
    В худшем случае сложность O(Q)
    */
    while ( !bookings_.empty()
    && bookings_.front().booking_time <= booking_time - BOOKING_MAX_PERIOD  ) {
      removeLastBooking(); //Сложность O(log Q )
    }
  }

public:

  Hotel():
    rooms_booking_count_(0)
    {}

  /*
   Каждый раз при подсчете числа комнат и клиентов вызываем функцию
   checkBookings для проверки и очистики очереди бронирований от
   "старых" бронирований
  */

  size_t getClientsCount( int64_t booking_time ) {
    checkBookings(booking_time); //Сложность O(Q * logQ)
    return clients_.size();
  }

  size_t getRoomsCount( int64_t book_time ) {
    checkBookings( book_time ); //Сложность O(Q * logQ)
    return rooms_booking_count_;
  }
  /*
  При каждом бронировании выполняются следующие действия:
  Увеличиваем счетчик забронированных комнат
  Увеличиваем счетчик числа бронирований комнат для каждого клиента
  Добавляем данные о бронировании в очередь бронирований
  */
  void book( const Booking& booking ) {

    rooms_booking_count_ += booking.rooms_count;
    clients_[booking.client_id] += booking.rooms_count; //Сложность O( logQ )
    bookings_.push( booking ); // Сложность  O(1)

  }
};

class BookingManager {
private:
  int64_t last_time_; //Время последнего события
  map<string, Hotel> hotels_;
public:

  BookingManager():
    last_time_(0)
    {}

  void bookHotel( int64_t book_time, const string& hotel_name, uint client_id, uint rooms_count ) {
    last_time_ = book_time;
    hotels_[hotel_name].book( {book_time, client_id, rooms_count} ); // O(logQ) + O(logQ)
  }

  size_t getHotelClientsCount( const string &hotel_name ) { // Сложность O( Q*logQ )
      return hotels_[hotel_name].getClientsCount( last_time_ );
  }


  size_t getHotelRoomsCount( const string &hotel_name ) { // Сложность O( Q*logQ )
      return hotels_[hotel_name].getRoomsCount( last_time_ );
  }

};


int main() {

  BookingManager booking_manager;

  /*
   По условиям задачи проверять корректность входных данных не нужно
   */

  int q;
  cin >> q;
  for (int i=0; i < q; ++i ) { //Сложность O(N)

    string cmd_type = "";
    cin >> cmd_type;

    if ( cmd_type == "BOOK" ) {
      int64_t book_time;
      string hotel_name ="";
      uint client_id, rooms_count;
      cin >> book_time >> hotel_name >> client_id >> rooms_count;
      booking_manager.bookHotel( book_time, hotel_name, client_id, rooms_count  );

    } else if ( cmd_type == "CLIENTS" ) {
      string hotel_name = "";
      cin >>  hotel_name;
      cout << booking_manager.getHotelClientsCount( hotel_name ) << endl;

    } else if ( cmd_type == "ROOMS") {
      string hotel_name = "";
      cin >> hotel_name;
      cout << booking_manager.getHotelRoomsCount( hotel_name ) << endl;

    } else {
      abort();
    }
  }
  return 0;
}
