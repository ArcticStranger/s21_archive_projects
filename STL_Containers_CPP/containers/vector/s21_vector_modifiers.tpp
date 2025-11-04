#ifndef _S21_VECTOR_MODS_
#define _S21_VECTOR_MODS_

namespace s21 {
// чистка вектора
template <typename T>
void vector<T>::clear() {
  size_ = 0;
}
// вставка value на конкретную позицию
template <typename T>
typename vector<T>::iterator vector<T>::insert(
    vector<T>::iterator pos, vector<T>::const_reference value) {
  size_type index = pos - begin();
  // если вышли за пределы
  if (index > size_) {
    throw std::out_of_range(
        "Position of inserting the value is out of range of the current vector "
        "memory");
  }
  // если максимум соответствует текущему размеру то максимум расширяется
  if (size_ == capacity_) {
    reserve(std::max(
        capacity_ * 2,
        size_t(1)));  // если вдруг вектор пустой то определяет 1 в размере
  }

  iterator new_pos = begin() + index;

  for (iterator i = end(); i != new_pos; --i) {
    *i = *(i - 1);  // смещение элементов влево от позиции которую будет
                    // занимать теперь value
  }
  *new_pos = value;  // значение занимает то самое место по pos
  ++size_;  // размер вложенных элементов увеличичается на 1 раз уж добавили
            // один value

  return new_pos;
}
// удаляет элемент по позиции pos
template <typename T>
void vector<T>::erase(vector<T>::iterator pos) {
  size_type position = pos - data_;

  if (position >= size_) {
    throw std::out_of_range("Error: index is out of range");
  }
  // смещает вправо элементы
  for (iterator i = pos; i != end() - 1; ++i) {
    *i = *(i + 1);
    ;
  }
  --size_;  // уменьшает размер на 1
}

// добавление элемента к концу
template <typename T>
void vector<T>::push_back(const_reference value) {
  if (size_ == capacity_) {
    reserve(std::max(capacity_ * 2, size_t(1)));
  }
  data_[size_] = value;
  ++size_;
}

// удаление последнего элемента
template <typename T>
void vector<T>::pop_back() {
  if (size_ > 0) {
    --size_;
    // так как размер вложенных элементов смеситлся на 1
    // то и последний элемент тоже сотрется
  }
}

// смена данных одного вектора с другим
template <typename T>
void vector<T>::swap(vector<T>& other) {
  std::swap(data_, other.data_);
  std::swap(size_, other.size_);
  std::swap(capacity_, other.capacity_);
}

}  // namespace s21

#endif