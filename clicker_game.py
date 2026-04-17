import tkinter as tk
from tkinter import messagebox
import random


class ClickerGame:
    def __init__(self, root):
        self.root = root
        self.root.title("Бой с монстром")
        self.root.geometry("500x400")
        
        # Переменные игры
        self.coins = 0
        self.click_power = 1
        
        # Параметры монстра
        self.monster_max_health = 100
        self.monster_health = self.monster_max_health
        self.monster_level = 1
        
        # Параметры игрока
        self.player_max_health = 100
        self.player_health = self.player_max_health
        
        # Стоимость зелья
        self.potion_cost = 20
        self.potion_heal = 30
        
        # Создаем элементы интерфейса
        self.create_widgets()
    
    def create_widgets(self):
        # Метка со счетом (монетки)
        self.coins_label = tk.Label(
            self.root, 
            text=f"Монетки: {self.coins}", 
            font=("Arial", 20),
            fg="gold"
        )
        self.coins_label.pack(pady=10)
        
        # Здоровье игрока
        self.player_health_frame = tk.Frame(self.root)
        self.player_health_frame.pack(pady=5)
        
        self.player_health_label = tk.Label(
            self.player_health_frame,
            text=f"Здоровье игрока: {self.player_health}/{self.player_max_health}",
            font=("Arial", 14),
            fg="red"
        )
        self.player_health_label.pack(side=tk.LEFT, padx=10)
        
        # Здоровье монстра
        self.monster_health_frame = tk.Frame(self.root)
        self.monster_health_frame.pack(pady=5)
        
        self.monster_health_label = tk.Label(
            self.monster_health_frame,
            text=f"Здоровье монстра: {self.monster_health}/{self.monster_max_health}",
            font=("Arial", 14),
            fg="green"
        )
        self.monster_health_label.pack(side=tk.LEFT, padx=10)
        
        self.monster_level_label = tk.Label(
            self.monster_health_frame,
            text=f"Уровень: {self.monster_level}",
            font=("Arial", 12)
        )
        self.monster_level_label.pack(side=tk.LEFT, padx=10)
        
        # Кнопка для атаки
        self.attack_button = tk.Button(
            self.root,
            text="АТАКОВАТЬ МОНСТРА!",
            font=("Arial", 18),
            bg="#FF5722",
            fg="white",
            width=20,
            height=2,
            command=self.on_attack
        )
        self.attack_button.pack(pady=20)
        
        # Кнопка покупки зелья
        self.potion_button = tk.Button(
            self.root,
            text=f"Купить зелье (+{self.potion_heal} HP)",
            font=("Arial", 14),
            bg="#4CAF50",
            fg="white",
            command=self.buy_potion
        )
        self.potion_button.pack(pady=10)
        
        # Метка стоимости зелья
        self.potion_cost_label = tk.Label(
            self.root,
            text=f"Стоимость зелья: {self.potion_cost} монет",
            font=("Arial", 12)
        )
        self.potion_cost_label.pack()
        
        # Метка силы клика
        self.power_label = tk.Label(
            self.root,
            text=f"Сила атаки: {self.click_power}",
            font=("Arial", 12)
        )
        self.power_label.pack(pady=5)
        
        # Сообщение о последнем действии
        self.message_label = tk.Label(
            self.root,
            text="",
            font=("Arial", 12),
            fg="blue"
        )
        self.message_label.pack(pady=10)
    
    def on_attack(self):
        if self.player_health <= 0:
            messagebox.showinfo("Игра окончена", "Вы проиграли! Монстр победил вас.")
            self.reset_game()
            return
            
        if self.monster_health <= 0:
            self.spawn_new_monster()
            return
        
        # Случайный урон монстру (от click_power до click_power * 2)
        damage_to_monster = random.randint(self.click_power, self.click_power * 2)
        self.monster_health -= damage_to_monster
        
        # Случайные монетки (от 1 до 5 + уровень монстра)
        coins_earned = random.randint(1, 5) + self.monster_level
        self.coins += coins_earned
        
        # Монстр атакует в ответ с случайным уроном
        monster_damage = random.randint(1, 5 + self.monster_level * 2)
        self.player_health -= monster_damage
        
        # Обновляем сообщение
        message = f"Вы нанесли {damage_to_monster} урона! Получено {coins_earned} монет. Монстр нанес {monster_damage} урона."
        self.message_label.config(text=message)
        
        # Проверяем состояние монстра
        if self.monster_health <= 0:
            self.monster_health = 0
            message = f"Монстр повержен! Вы получили бонус {coins_earned * 2} монет!"
            self.message_label.config(text=message)
            self.coins += coins_earned  # Бонус за победу
            self.root.after(1000, self.spawn_new_monster)
        
        # Проверяем состояние игрока
        if self.player_health <= 0:
            self.player_health = 0
            messagebox.showinfo("Игра окончена", "Вы проиграли! Монстр победил вас.")
            self.reset_game()
        
        self.update_display()
    
    def spawn_new_monster(self):
        self.monster_level += 1
        self.monster_max_health = 100 + (self.monster_level - 1) * 50
        self.monster_health = self.monster_max_health
        self.click_power += 1  # Увеличиваем силу атаки с каждым уровнем
        self.message_label.config(text=f"Новый монстр {self.monster_level} уровня появился!")
        self.update_display()
    
    def buy_potion(self):
        if self.coins >= self.potion_cost:
            if self.player_health >= self.player_max_health:
                messagebox.showinfo("Полное здоровье", "У вас уже полное здоровье!")
                return
            
            self.coins -= self.potion_cost
            old_health = self.player_health
            self.player_health = min(self.player_max_health, self.player_health + self.potion_heal)
            healed = self.player_health - old_health
            self.message_label.config(text=f"Вы выпили зелье и восстановили {healed} здоровья!")
            self.update_display()
        else:
            messagebox.showinfo("Недостаточно монет", 
                              f"Нужно {self.potion_cost} монет для зелья!")
    
    def reset_game(self):
        self.coins = 0
        self.click_power = 1
        self.monster_level = 1
        self.monster_max_health = 100
        self.monster_health = self.monster_max_health
        self.player_max_health = 100
        self.player_health = self.player_max_health
        self.message_label.config(text="")
        self.update_display()
    
    def update_display(self):
        self.coins_label.config(text=f"Монетки: {self.coins}")
        self.player_health_label.config(text=f"Здоровье игрока: {self.player_health}/{self.player_max_health}")
        self.monster_health_label.config(text=f"Здоровье монстра: {self.monster_health}/{self.monster_max_health}")
        self.monster_level_label.config(text=f"Уровень: {self.monster_level}")
        self.potion_cost_label.config(text=f"Стоимость зелья: {self.potion_cost} монет")
        self.power_label.config(text=f"Сила атаки: {self.click_power}")


if __name__ == "__main__":
    root = tk.Tk()
    game = ClickerGame(root)
    root.mainloop()
